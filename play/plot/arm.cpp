#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>
using namespace Eigen;
decltype(auto) rtt(float x, float y, float z)
{
    auto m = AngleAxisd(x * M_PI, Vector3d::UnitX())   //
                 * AngleAxisd(y * M_PI, Vector3d::UnitY()) //
                 * AngleAxisd(z * M_PI, Vector3d::UnitZ());
    return m;
}
int main(int argc, const char **argv)
{
    Vector3d joint1;
    joint1 << 0, 0, 0;
    Vector3d joint2;
    joint2 << 2, 0, 0;
    Vector3d joint3;
    joint3 << 4, 0, 0;
    Matrix3d m;
    // m = AngleAxisd(0.3 * M_PI, Vector3d::UnitX())   //
    //              * AngleAxisd(0.3 * M_PI, Vector3d::UnitY()) //
    //              * AngleAxisd(0.3 * M_PI, Vector3d::UnitZ());
    m = rtt(0,0,0.4);
    std::ofstream ofs("d.dat");
    ofs << (joint1).transpose() << std::endl;
    ofs << (m * joint2).transpose() << std::endl;
    ofs << (m * rtt(0,0,-0.3) * joint3).transpose() << std::endl;
    ofs.close();
    system("sleep 0.1");
    system("gnuplot-x11 -persist a.plt");
    return 0;
}