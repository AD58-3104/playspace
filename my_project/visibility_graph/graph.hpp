#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>



using coord = pair<double, double>;


struct shape
{
    using namespace std;
    std::vector<coord> points;
    void print()
    {
        // std::sort(points.begin(), points.end());
        // for (int i = 0; i < points.size(); i++)
        // {
        //     for (int j = 0; j < i; ++j)
        //     {
        //         if (i == j)
        //             continue;
        //         // std::ofs << points[i].first << " " << points[i].second << " " << points[j].first << " " << points[j].second << std::endl;
        //     }
        //     std::ofs << points[i].first << " " << points[i].second << std::endl;
        // }
        // std::ofs << points[0].first << " " << points[0].second << std::endl;
    }
};

class visibilityGraph
{
public:
    std::vector<shape> obstacles;
};