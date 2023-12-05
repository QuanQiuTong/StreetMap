#include "osm.h"

#define foreach_sibling(p, t) for (; p && ((TiXmlNode *)p)->ValueTStr() == t; p = p->NextSiblingElement())
#define foreach_child(p, q, t) for (auto q = p->FirstChildElement(t); q; q = q->NextSiblingElement(t))

static const char *Attribute(TiXmlElement *p, const char *attr)
{
    const char *ret = p->Attribute(attr);
    if (!ret)
    {
        fprintf(stderr, "Attribute %s not found", attr);
        throw std::invalid_argument("Attribute not found");
    }
    return ret;
}
static auto id(TiXmlElement *p) { return atoll(Attribute(p, "id")); }
static auto lon(TiXmlElement *p) { return atof(Attribute(p, "lon")); }
static auto lat(TiXmlElement *p) { return atof(Attribute(p, "lat")); }
static auto ref(TiXmlElement *p) { return atoll(Attribute(p, "ref")); }
static std::string k(TiXmlElement *p) { return Attribute(p, "k"); }
static std::string v(TiXmlElement *p) { return Attribute(p, "v"); }
static std::string type(TiXmlElement *p) { return Attribute(p, "type"); }
static std::string role(TiXmlElement *p) { return Attribute(p, "role"); }

NAMESPACE_OSM

template <template <typename Key, typename Val> typename AssocCon>
Bounds parse(FILE *infile, AssocCon<ll, Node> &nodes, AssocCon<ll, Way> &ways, AssocCon<ll, Relation> &relations)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(infile))
        throw std::invalid_argument("File not found");

    auto p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    Bounds bounds = {atof(Attribute(p, "minlat")),
                     atof(Attribute(p, "minlon")),
                     atof(Attribute(p, "maxlat")),
                     atof(Attribute(p, "maxlon"))};
    p = p->NextSiblingElement();
    nodes.clear();
    foreach_sibling(p, "node")
        nodes[id(p)] = {lon(p), lat(p)};
    ways.clear();
    foreach_sibling(p, "way")
    {
        auto &w = ways[id(p)];
        auto q = p->FirstChildElement();
        foreach_sibling(q, "nd") w.nd.push_back(nodes[ref(q)]);
        foreach_sibling(q, "tag") w.tag[k(q)] = v(q);
    }
    relations.clear();
    foreach_sibling(p, "relation")
    {
        auto &r = relations[id(p)];
        auto q = p->FirstChildElement();
        foreach_sibling(q, "member") r.member.push_back({type(q), ref(q), role(q)});
        foreach_sibling(q, "nd") r.tag[k(q)] = v(q);
    }
    return bounds;
}

END_NAMESPACE_OSM

#ifdef TEST_OSM
#include <algorithm>
int main()
{
    FILE *infile = fopen("map.osm", "r");
    using namespace osm;
    std::map<ll, Node> nodes;
    std::map<ll, Way> ways;
    std::map<ll, Relation> relations;
    parse(infile, nodes, ways, relations);

    std::map<std::string, size_t> tag_k, tag_v;
    for(auto [id,way]:ways){
        for(auto [k,v]:way.tag){
            tag_k[k]++;
            tag_v[v]++;
        }
    }
    //rearrange so that the most frequent tags are at the front
    std::vector<std::pair<std::string,size_t>> tag_k_vec(tag_k.begin(),tag_k.end());
    std::vector<std::pair<std::string,size_t>> tag_v_vec(tag_v.begin(),tag_v.end());
    std::sort(tag_k_vec.begin(),tag_k_vec.end(),[](auto a,auto b){return a.second>b.second;});
    std::sort(tag_v_vec.begin(),tag_v_vec.end(),[](auto a,auto b){return a.second>b.second;});
    for(auto [k,v]:tag_k_vec)if(v>100){
        printf("%s: %zu\n",k.c_str(),v);
    }
    puts("====================================");
    for(auto [k,v]:tag_v_vec)if(v>100){
        printf("%s: %zu\n",k.c_str(),v);
    }
    puts("success");
    return 0;
}
#endif