#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <thread>
#include <chrono>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "cv.hpp"

using graph_point_t = std::pair<double, double>;
using graph_edge_t = std::pair<graph_point_t, graph_point_t>;

class drawing;

struct shape
{
    std::vector<graph_point_t> points;
    std::vector<graph_edge_t> edges;

    void print()
    {
        for (const auto &[x, y] : points)
        {
            std::cout << x << " " << y << std::endl;
        }
    }

    void addRectangle(graph_point_t centre, double length, double rad)
    {
        const auto [x, y] = centre;
        using std::sin, std::cos;
        const int sign1[4] = {1, -1, 1, -1};
        const int sign2[4] = {1, 1, -1, -1};
        for (int i = 0; i < 4; ++i)
            points.emplace_back(x + (sign1[i] * length / 2) * cos(rad) + y + (sign2[i] * length / 2) * (-sin(rad)), x + (sign1[i] * length / 2) * sin(rad) + y + (sign2[i] * length / 2) * cos(rad));
    };

    void declination_sort()
    {
        std::sort(points.begin(), points.end(), [](const graph_point_t &lhs, const graph_point_t &rhs)
                  { return lhs.second < rhs.second; });
        const auto [max_x, max_y] = *(points.begin());
        std::sort(points.begin(), points.end(), [max_x, max_y](const graph_point_t &lhs, const graph_point_t &rhs)
                  { return std::atan2(lhs.second - max_y, lhs.first - max_x) < std::atan2(rhs.second - max_y, rhs.first - max_x); });
    }

    void makeEdges()
    {
        if (points.size() <= 1)
        {
            return;
        }
        if (edges.size() != 0)
        {
            edges.clear();
        }
        for (int i = 1; i < points.size(); ++i)
        {
            edges.emplace_back(points[i - 1], points[i]);
        }
        edges.emplace_back(points[points.size() - 1], points[0]);
    };
};

std::vector<graph_point_t> declination_sort_by_ref_point(const graph_point_t &ref_p, const std::vector<graph_point_t> &other_points)
{
    std::vector<graph_point_t> result_points = other_points;
    auto [ref_x, ref_y] = ref_p;
    std::sort(result_points.begin(), result_points.end(), [ref_x, ref_y](const graph_point_t &lhs, const graph_point_t &rhs)
              { return std::atan2(lhs.second - ref_y, lhs.first - ref_x) < std::atan2(rhs.second - ref_y, rhs.first - ref_x); });
    result_points.erase(result_points.begin()); // ref_pと同じ点を削除
    return result_points;
}

class visibilityGraph
{
public:
    void addAllEdgesAndPoints()
    {
        obstacle_points.clear();
        obstacle_edges.clear();
        obstacle_points.resize(300); //適当
        obstacle_edges.resize(300);  //適当
        for (const auto &itm : obstacles)
        {
            obstacle_points.insert(obstacle_points.end(), itm.points.begin(), itm.points.end());
            obstacle_edges.insert(obstacle_edges.end(), itm.edges.begin(), itm.edges.end());
        }
    }
    void add_obstacles(shape &&obstacle)
    {
        obstacles.emplace_back(std::forward<shape>(obstacle));
    }
    void makeVisibilityGraph()
    {
        using distance = double;
        using end_p = std::pair<graph_point_t,const graph_edge_t&>;
        const auto [ref_x, ref_y] = self_location;
        auto henkakuCmp = [ref_x, ref_y](const end_p &lhs, const end_p &rhs)
        {
            return std::atan2(lhs.first.second - ref_y, lhs.first.first - ref_x) < std::atan2(rhs.first.second - ref_y, rhs.first.first - ref_x);
        };
        std::vector<end_p> tmp;
        for(const auto&itm:obstacle_edges){
            tmp.emplace_back(  )
        }
        std::priority_queue<end_p,std::vector<end_p>,decltype(henkakuCmp)> henkaku_que(henkakuCmp);
        
    }
    std::vector<graph_point_t> obstacle_points;
    std::vector<graph_edge_t> obstacle_edges;
    std::vector<graph_edge_t> visible_edge;
    std::vector<shape> obstacles;

    graph_point_t self_location;

private:
};

class drawing
{
public:
    drawing() : img(cv::Mat::zeros(1000, 1000, CV_8UC3)), color(125, 50, 50)
    {
        // cv::line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 5, 8);
        // fillConvexPolyとpolylines
    }

    void display(const visibilityGraph &obj)
    {
        writeShape(obj.obstacles);
        using namespace std::literals;
        std::this_thread::sleep_for(400ms);
        cv::imshow("visibility graph", img);
        // cv::moveWindow( "visibility graph", 0, 200 );
        cv::waitKey(0);
    }

private:
    cv::Mat img;
    const cv::Scalar color;
    void writeShape(const std::vector<shape> &shapes)
    {
        std::vector<std::vector<cv::Point>> objects_points;
        for (const auto &itm : shapes)
        {
            std::vector<cv::Point> tmp;
            for (const auto &point : itm.points)
            {
                tmp.push_back(cv::Point(static_cast<int>(point.first), static_cast<int>(point.second)));
            }
            objects_points.emplace_back(std::move(tmp));
        }

        for (const std::vector<cv::Point> &obj : objects_points)
        {
            cv::fillConvexPoly(img, obj, color);
        }
    }
};

#endif // !GRAPH_H_