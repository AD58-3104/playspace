#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
using namespace std;
int main(int argc, const char **argv)
{
    ofstream ofs("d.dat");
    for (int i = 0; i < 2 * 314; i+=40)
    {
        ofs << (double)(i / 100.0) << " " << sin((double)i / 100.0) << std::endl;
    }
    ofs.close();
    system("gnuplot-x11 -persist a.plt");
    // FILE* gp = popen("gnuplot-x11 -persist","w");
    // if(gp == NULL)
    //     cout << "error\n";
    // fprintf(gp,"plot \"d.dat\" with linespoints ps 5 pt 7");
    // fprintf(gp,"plot sin(x)");
    // pclose(gp);
    return 0;
}