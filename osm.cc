#include "tinyxml.h"

/**
 * The following cpp-files are included directly,
 * because they can not be compiled correctly with makefile.
 */
#include "tinyxml.cpp"
#include "tinyxmlparser.cpp"
#include "tinyxmlerror.cpp"

#include "osm.h"

#define forSibling(p, t) for (; p && ((TiXmlNode *)p)->ValueTStr() == t; p = p->NextSiblingElement())
// #define foreach_child(p, q, t) for (auto q = p->FirstChildElement(t); q; q = q->NextSiblingElement(t))

static const char *Attr(TiXmlElement *p, const char *attr)
{
    const char *ret = p->Attribute(attr);
    if (!ret)
        throw std::invalid_argument("Attribute not found");
    return ret;
}
static long long id(TiXmlElement *p) { return atoll(Attr(p, "id")); }
static long long ref(TiXmlElement *p) { return atoll(Attr(p, "ref")); }
static std::string k(TiXmlElement *p) { return Attr(p, "k"); }
static std::string v(TiXmlElement *p) { return Attr(p, "v"); }

NAMESPACE_OSM

AssocCon<ll, Node> nodes;
AssocCon<ll, Way> ways;
AssocCon<ll, Relation> relations;
Bounds parse(FILE *infile)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(infile))
        throw std::invalid_argument("File not found");
    nodes.clear(), ways.clear(), relations.clear();
    auto p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    Bounds bounds = {atof(Attr(p, "minlat")), atof(Attr(p, "minlon")),
                     atof(Attr(p, "maxlat")), atof(Attr(p, "maxlon"))};
    p = p->NextSiblingElement();
    forSibling(p, "node")
        nodes[id(p)] = {atof(Attr(p, "lon")), atof(Attr(p, "lat"))};
    forSibling(p, "way")
    {
        auto &w = ways[id(p)];
        auto q = p->FirstChildElement();
        forSibling(q, "nd") w.nd.push_back(nodes[ref(q)]);
        forSibling(q, "tag") w.tag[k(q)] = v(q);
    }
    forSibling(p, "relation")
    {
        auto &r = relations[id(p)];
        auto q = p->FirstChildElement();
        forSibling(q, "member") r.member.push_back({Attr(q, "type"), ref(q), Attr(q, "role")});
        forSibling(q, "nd") r.tag[k(q)] = v(q);
    }
    return bounds;
}

END_NAMESPACE_OSM

// #define TEST_OSM
#ifdef TEST_OSM
#include <algorithm>
// #include "tinyxml.cpp"
// #include "tinyxmlparser.cpp"
// #include "tinyxmlerror.cpp"

int main()
{
    FILE *infile = fopen("map.osm", "r");
    using namespace osm;
    parse(infile);

    std::map<std::string, size_t> tag_k, tag_v;
    for (auto [id, way] : ways)
    {
        for (auto [k, v] : way.tag)
        {
            tag_k[k]++;
            tag_v[v]++;
        }
    }
    // rearrange so that the most frequent tags are at the front
    std::vector<std::pair<std::string, size_t>> tag_k_vec(tag_k.begin(), tag_k.end());
    std::vector<std::pair<std::string, size_t>> tag_v_vec(tag_v.begin(), tag_v.end());
    std::sort(tag_k_vec.begin(), tag_k_vec.end(), [](auto a, auto b)
              { return a.second > b.second; });
    std::sort(tag_v_vec.begin(), tag_v_vec.end(), [](auto a, auto b)
              { return a.second > b.second; });
    for (auto [k, v] : tag_k_vec)
        if (v > 100)
        {
            printf("%s: %zu\n", k.c_str(), v);
        }
    puts("====================================");
    for (auto [k, v] : tag_v_vec)
        if (v > 100)
        {
            printf("%s: %zu\n", k.c_str(), v);
        }
    puts("success");
    return 0;
}
#endif
