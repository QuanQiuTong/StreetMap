#include "osm.cc"
#include <cmath>
#include <algorithm>
#include <map>
using namespace std;
static constexpr double rad(double deg) { return deg * M_PI / 180; }
static double realDistance(double lon1, double lat1, double lon2, double lat2)
{
    constexpr double R = 6378137 /*metres*/;

    double dLat = rad(lat2 - lat1);
    double dLon = rad(lon2 - lon1);
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(rad(lat1)) * cos(rad(lat2)) *
                   sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a)); // asin(sqrt(a)) * 2
    return R * c;
}
int main(){
    double testdis = realDistance(121.910310, 30.858038, 122.011323, 30.663016);
    printf("%lf\n", testdis);
}
int main1()
{
    FILE *infile = fopen("map.osm", "r");
    assert(infile);
    using namespace osm;

    parse(infile);

    map<string, size_t> tag_k, tag_v;
    for (auto [id, way] : ways)
    {
        if (way.tag.empty())
            tag_k["nontag"]++;
        else if (way.tag.count("highway"))
            tag_k["highway"]++;
        else if (way.tag.count("building"))
            tag_k["building"]++;
        else
            for(auto [k, v] : way.tag)
                tag_k[k]++;
    }
    // rearrange so that the most frequent tags are at the front
    vector<pair<string, size_t>> tag_k_vec(tag_k.begin(), tag_k.end());
    sort(tag_k_vec.begin(), tag_k_vec.end(), [](auto a, auto b)
         { return a.second > b.second; });

    printf("nontag: %zu\n", tag_k["nontag"]);
    for (auto [k, v] : tag_k_vec)
        printf("%s: %zu\n", k.c_str(), v);
    return 0;
}
int main0()
{
    FILE *infile = fopen("map.osm", "r");
    assert(infile);
    using namespace osm;
    parse(infile);

    std::map<std::string, size_t> tag_k, tag_v;
    size_t nontag = 0;
    for (auto [id, way] : ways)
    {
        if (way.tag.empty())
        {
            nontag++;
            continue;
        }
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

    printf("nontag: %zu\n", nontag);
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
