#include "graph.hpp"
#include "cv.hpp"

int main(int argc, char const *argv[])
{
    std::vector<shape> obstacles;
    shape tri;
    tri.points = {std::make_pair(100, 200), std::make_pair(400, 500), std::make_pair(200, 800), std::make_pair(300, 300)}; //ならない
    tri.sort();
    shape aaa;
    aaa.points = {std::make_pair(100, 100), std::make_pair(150, 200), std::make_pair(300, 300), std::make_pair(400, 100)}; //綺麗になる
    aaa.sort();
    for (auto [a, b] : tri.points)
    {
        std::cout << a << " " << b << std::endl;
    }
    for (auto [a, b] : aaa.points)
    {
        std::cout << a << " " << b << std::endl;
    }
    obstacles.push_back(tri);
    obstacles.push_back(aaa);
    drawing draw;
    draw.display(obstacles);
    return 0;
}
