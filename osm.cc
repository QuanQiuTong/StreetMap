#include "tinyxml.h"

#include <stdexcept>

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

static const char *att(TiXmlElement *p, const char *attr)
{
    const char *ret = p->Attribute(attr); // printf("# %s - %s\n", attr, ret);
    if (!ret)
        throw std::invalid_argument("Attribute "+std::string(attr)+" not found");
    return ret;
}
static double attd(TiXmlElement *p, const char *attr) { return atof(att(p, attr)); }
static long long id(TiXmlElement *p) { return atoll(att(p, "id")); }
static long long ref(TiXmlElement *p) { return atoll(att(p, "ref")); }

NAMESPACE_OSM

double minlat, minlon, maxlat, maxlon;
AssocCon<ll, Node> nodes;
AssocCon<ll, Way> ways;
AssocCon<ll, Relation> relations;
void parse(FILE *infile)
{
    TiXmlDocument doc;
    if (!doc.LoadFile(infile))
        throw std::invalid_argument("File not found");
    auto p = doc.RootElement()->FirstChildElement();
    while (p && ((TiXmlNode *)p)->ValueTStr() != "bounds")
        p = p->NextSiblingElement();
    minlat = attd(p, "minlat"), minlon = attd(p, "minlon"), maxlat = attd(p, "maxlat"), maxlon = attd(p, "maxlon");
    p = p->NextSiblingElement();
    nodes.clear(), ways.clear(), relations.clear();
    forSibling(p, "node") nodes.insert({id(p), {attd(p, "lon"), attd(p, "lat")}});
    forSibling(p, "way")
    {
        auto &w = ways[id(p)];
        auto q = p->FirstChildElement();
        forSibling(q, "nd") w.nd.push_back(nodes[ref(q)]);
        forSibling(q, "tag") w.tag.insert({att(q, "k"), att(q, "v")});
    }
    forSibling(p, "relation")
    {
        auto &r = relations[id(p)];
        auto q = p->FirstChildElement();
        forSibling(q, "member") r.member.push_back({att(q, "type"), ref(q), att(q, "role")});
        forSibling(q, "nd") r.tag.insert({att(q, "k"), att(q, "v")});
    }
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
