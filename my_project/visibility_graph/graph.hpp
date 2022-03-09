#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

using coord = std::pair<int64_t, int64_t>;


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
    void sort(){
        std::sort(points.begin(),points.end(),[](const std::pair<double,double>& lhs,const std::pair<double,double>& rhs){
            if(lhs.second != rhs.second){
                return lhs.second < rhs.second;
            }
            else{
                return lhs.first < rhs.first;
            }
        });
    }
};

class visibilityGraph
{
public:
    std::vector<shape> obstacles;
};

#endif // !GRAPH_H_