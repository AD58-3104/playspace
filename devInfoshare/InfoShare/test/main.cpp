#include <iostream>
#include <string>
#include "infoshare.h"
#include "infoshare.pb.h"
using namespace std;

struct St
{
#ifdef __cpp_inline_variables
    inline static constexpr int I = 0b1111'1111;
#else
    static const int I = 0b1111'1001;
#endif
};
Citbrains::infosharemodule::InfoShare info;
using namespace std::literals::chrono_literals;

int main(int argc, char const *argv[])
{
    int id = 1;
    info.setup(Citbrains::Udpsocket::SocketMode::unicast_mode, id, COLOR_MAGENTA, "127.0.0.1", 7110);
    assert(info.getOurcolor() == COLOR_MAGENTA);
    assert(info.getID() == id);
    Pos2D pos2d(50,50,90);
    Pos2DCf pos2dcf(pos2d,30,0);
    std::vector<Pos2D> v{pos2d};
    info.sendCommonInfo(pos2dcf,pos2dcf,v,v,v,20,"saiaku na debug","mendokusa",v, );
    std::this_thread::sleep_for(4000ms);
    for (int i = 1; i <= 4;i++)
    {
        std::cout << "------------this is no " << i << " infomation --------------------" << std::endl;
        std::cout << "cf_own :" << info.getcf_own(i) << std::endl;
        std::cout << "votage :" << info.getvoltage(i) << std::endl;
        std::cout << "cf_ball :" << info.getcf_ball(i) << std::endl;
        std::cout << "fps :" << info.getfps(i) << std::endl;
        std::cout << "status :" << info.getstatus(i) << std::endl;
        std::cout << "temperature :" << info.gettemperature(i) << std::endl;
        std::cout << "highest_servo :" << info.gethighest_servo(i) << std::endl;
    }
    return 0;
}
