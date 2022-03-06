#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
using namespace std;
using coord = pair<double, double>;

ofstream ofs("data.dat");

struct shape
{
    vector<coord> points;
    void print()
    {
        sort(points.begin(),points.end());
        for (int i = 0; i < points.size(); i++)
        {
            for (int j = 0; j < i; ++j)
            {
                if (i == j)
                    continue;
                // ofs << points[i].first << " " << points[i].second << " " << points[j].first << " " << points[j].second << std::endl;
            }
            ofs << points[i].first << " " << points[i].second << std::endl; 
        }
        ofs << points[0].first << " " << points[0].second << std::endl;
    }
    void pri(){
    }
};

int main(int argc, char const *argv[])
{
    shape tri;
    tri.points = {make_pair(1, 2), make_pair(4, 5), make_pair(2, 8),make_pair(3,3)};
    tri.print();
    system("gnuplot -persist show.plt");
    return 0;
}
