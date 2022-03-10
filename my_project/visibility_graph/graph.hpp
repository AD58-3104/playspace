#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <algorithm>

using coord = std::pair<double, double>;

struct shape
{
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
    void declination_sort()
    {
        std::sort(points.begin(),points.end(),[](const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
                { return lhs.second < rhs.second; });
        auto [max_x,max_y] = *(points.begin());
        std::cout << max_x << max_y << std::endl;
        std::sort(points.begin(), points.end(), [max_x,max_y](const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
                { return std::atan2(lhs.second - max_y, lhs.first - max_x) < std::atan2(rhs.second - max_y, rhs.first - max_x); });
    }
};

class visibilityGraph
{
public:
    std::vector<shape> obstacles;
};

#endif // !GRAPH_H_