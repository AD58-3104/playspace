#include "graph.hpp"
#include "cv.hpp"

int main(int argc, char const *argv[])
{
    visibilityGraph graph;
    
    // shape tri;
    // tri.points = {std::make_pair(100, 200), std::make_pair(400, 500), std::make_pair(200, 800), std::make_pair(300, 300)}; //ならない
    // tri.declination_sort();
    
    // shape aaa;
    // aaa.points = {std::make_pair(100, 100), std::make_pair(150, 200), std::make_pair(300, 300), std::make_pair(400, 100)}; //綺麗になる
    // aaa.declination_sort();
    
    // shape thr;
    // thr.points = {std::make_pair(900,900),std::make_pair(800,700),std::make_pair(750,830)};
    // thr.declination_sort();

    // graph.add_obstacles(tri);
    // graph.add_obstacles(aaa);
    // graph.add_obstacles(thr);
    shape rec;
    rec.addRectangle({200,300},100,3.14/4.0);
    rec.declination_sort();
    graph.add_obstacles(std::move(rec));   
    drawing draw;
    draw.display(graph);

    return 0;
}
