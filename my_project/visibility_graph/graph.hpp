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
    std::vector<std::pair<coord,coord>> edges;

    void print()
    {
        for (const auto &[x, y] : points)
        {
            std::cout << x << " " << y << std::endl;
        }
    }

    void addRectangle(coord centre, double length, double rad)
    {
        double x = centre.first;
        double y = centre.second;
        using std::sin, std::cos;
        int sign1[4] = {1, -1, 1, -1};
        int sign2[4] = {1, 1, -1, -1};
        for (int i = 0; i < 4; ++i)
            points.emplace_back(x + (sign1[i] * length / 2) * cos(rad) + y + (sign2[i] * length / 2) * (-sin(rad)), x + (sign1[i] * length / 2) * sin(rad) + y + (sign2[i] * length / 2) * cos(rad));
    };

    void declination_sort()
    {
        std::sort(points.begin(), points.end(), [](const coord &lhs, const coord &rhs)
                  { return lhs.second < rhs.second; });
        auto [max_x, max_y] = *(points.begin());
        std::sort(points.begin(), points.end(), [max_x, max_y](const coord &lhs, const coord &rhs)
                  { return std::atan2(lhs.second - max_y, lhs.first - max_x) < std::atan2(rhs.second - max_y, rhs.first - max_x); });
    }

    void makeEdges(){
        if(points.size() <= 1){
            return ;
        }
        if(edges.size() != 0){
            edges.clear();
        }
        for(int i = 1; i < points.size();++i){
            edges.emplace_back(points[i - 1],points[i]);
        }
        edges.emplace_back(points[points.size() - 1],points[0]);
    };
};

std::vector<coord> declination_sort_by_ref_point(const coord &ref_p, const std::vector<coord> &other_points)
{
    std::vector<coord> result_points = other_points;
    auto [ref_x, ref_y] = ref_p;
    std::sort(result_points.begin(), result_points.end(), [ref_x, ref_y](const coord &lhs, const coord &rhs)
            { return std::atan2(lhs.second - ref_y, lhs.first - ref_x) < std::atan2(rhs.second - ref_y, rhs.first - ref_x); });
    result_points.erase(result_points.begin()); //ref_pと同じ点を削除
    return result_points;
}

class visibilityGraph
{
public:
    std::vector<shape> obstacles;
};

#endif // !GRAPH_H_