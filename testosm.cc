#include "osm.h"
#include <algorithm>
#include <map>
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
