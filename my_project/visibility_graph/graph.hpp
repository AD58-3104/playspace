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
        std::sort(points.begin(), points.end(), [](const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
                  { return lhs.second < rhs.second; });
        auto [max_x, max_y] = *(points.begin());
        std::sort(points.begin(), points.end(), [max_x, max_y](const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
                  { return std::atan2(lhs.second - max_y, lhs.first - max_x) < std::atan2(rhs.second - max_y, rhs.first - max_x); });
    }
};

class visibilityGraph
{
public:
    std::vector<shape> obstacles;
};

#endif // !GRAPH_H_