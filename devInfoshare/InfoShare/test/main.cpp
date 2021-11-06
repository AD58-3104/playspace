#include <iostream>
#include <string>
#include "infoshare.h"
#include "infoshare.pb.h"
using namespace std;

Citbrains::infosharemodule::InfoShare info;
using namespace std::literals::chrono_literals;

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
    int id = 1;
    info.setup(Citbrains::Udpsocket::SocketMode::broadcast_mode, id, COLOR_MAGENTA, "127.0.0.1");
    assert(info.getOurcolor() == COLOR_MAGENTA);
    std::cout << info.getOurcolor()<< std::endl;
    assert(info.getID() == id);
    Pos2D pos2d(50.0, 50.0, 90.0);
    Pos2DCf pos2dcf(pos2d, 30, 0);
    std::vector<Pos2D> v{pos2d};
    RobotStatus status;
    status.Temperature = 30;
    status.servo1temp = (45 << 8);
    status.MotorVoltage = (12 << 3);
    status.Posture = STATE_POSTURE_STAND;
    // info.sendCommonInfo(pos2dcf, pos2dcf, v, v, v, 20, "saiaku na debug", "mendokusa", v, status);
    std::this_thread::sleep_for(4000ms);
    for (int i = 2; i <= 2; i++)
    {
        std::cout << "------------this is no " << i << " infomation --------------------" << std::endl;
        std::cout << "cf_own :" << info.getcf_own(i) << std::endl;
        std::cout << "votage :" << info.getvoltage(i) << std::endl;
        std::cout << "cf_ball :" << info.getcf_ball(i) << std::endl;
        std::cout << "fps :" << info.getfps(i) << std::endl;
        std::cout << "status :" << info.getstatus(i) << std::endl;
        std::cout << "temperature :" << info.gettemperature(i) << std::endl;
        std::cout << "command :" << info.getcommand(i) << std::endl;
        std::cout << "current_behavior_name :" << info.getcurrent_behavior_name(i) << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^our_robot_gl :\n";
        for (const auto &itr : info.getour_robot_gl(i))
        {
            Pos2DPrint(itr);
        }
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^enemy_robot_gl :\n";
        for (const auto &itr : info.getenemy_robot_gl(i))
        {
            Pos2DPrint(itr);
        }
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^black_pole_gl :\n";
        for (const auto &itr : info.getblack_pole_gl(i))
        {
            Pos2DPrint(itr);
        }
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^target_pos_vec :\n";
        for (const auto &itr : info.gettarget_pos_vec(i))
        {
            Pos2DPrint(itr);
        }
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^self_pos_cf :\n";
        Pos2DCfPrint(info.getself_pos_cf(i));
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^ball_gl_cf :\n";
        Pos2DCfPrint(info.getball_gl_cf(i));
    }
    info.terminate();
    return 0;
}
