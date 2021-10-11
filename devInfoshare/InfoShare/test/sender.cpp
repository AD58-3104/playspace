#include "sUDPSocket.hpp"
using namespace Citbrains::Udpsocket;
using namespace std;
#include "infoshare.pb.h"

int main(int argc, char const *argv[])
{

    UDPClient client("127.0.0.1", 7110, SocketMode::unicast_mode);
    CitbrainsMessage::SharingData proto;
    char i ='a';
    std::string s = {i};
    proto.set_id(std::string{(char)2}.c_str());
    proto.set_cf_own(s.c_str());
    proto.set_cf_ball(s.c_str());
    proto.set_status(s.c_str());
    proto.set_fps(s.c_str());
    proto.set_voltage(s.c_str());
    proto.set_temperature(s.c_str());
    proto.set_highest_servo(s.c_str());
    // string command = "walking for ball";
    proto.set_command(s.c_str());
    // string beh = "attacker now";
    proto.set_current_behavior_name(s.c_str());
    s = proto.SerializeAsString();
    client.send(std::move(s));
    std::this_thread::sleep_for(1000ms);
    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    return 0;
}
