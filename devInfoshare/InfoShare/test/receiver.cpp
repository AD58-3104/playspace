#include "infoshare.h"
#include "sUDPSocket.hpp"
using namespace Citbrains::Udpsocket;
using namespace std;
#include "infoshare.pb.h"

void Pos2DPrint(const Pos2D &data)
{
    std::cout << "POS2D internal data is" << std::endl;
    std::cout << "x is " << data.x << "||| y is  " << data.y << "||| theta is " << data.th << std::endl;
}

void Pos2DCfPrint(const Pos2DCf &data)
{
    std::cout << "Pos2DCf internal data is " << std::endl;
    std::cout << "cf is " << data.cf << "||| is_detect " << std::boolalpha << data.is_detect << std::endl;
    Pos2DPrint(data.pos);
}

int main(int argc, char const *argv[])
{

    Citbrains::infosharemodule::InfoShare info;
    info.setup(Citbrains::Udpsocket::SocketMode::unicast_mode, 1, COLOR_MAGENTA, "127.0.0.1");
    std::this_thread::sleep_for(3000ms);
    std::cout << "-----------------------------------------\n";
    std::cout << "voltage :" << info.getvoltage(2) << std::endl;
    std::cout << "fps :" << info.getfps(2) << std::endl;
    std::cout << "status :" << info.getstatus(2) << std::endl;
    std::cout << "temperature :" << info.gettemperature(2) << std::endl;
    std::cout << "highest servo :" << info.gethighest_servo(2) << std::endl;
    std::cout << "command :" << info.getcommand(2) << std::endl;
    std::cout << "current_behavior_name :" << info.getcurrent_behavior_name(2) << std::endl;
    std::cout << "receive time :" << info.getrecv_time(2) << std::endl;
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^our_robot_gl :\n";
    for (const auto &itr : info.getour_robot_gl(2))
    {
        Pos2DPrint(itr);
    }
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^enemy_robot_gl :\n";
    for (const auto &itr : info.getenemy_robot_gl(2))
    {
        Pos2DPrint(itr);
    }
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^black_pole_gl :\n";
    for (const auto &itr : info.getblack_pole_gl(2))
    {
        Pos2DPrint(itr);
    }
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^target_pos_vec :\n";
    for (const auto &itr : info.gettarget_pos_vec(2))
    {
        Pos2DPrint(itr);
    }
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^self_pos_cf :\n";
    Pos2DCfPrint(info.getself_pos_cf(2));
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^ball_gl_cf :\n";
    Pos2DCfPrint(info.getball_gl_cf(2));
    info.terminate();
    return 0;
}
