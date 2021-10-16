#include <iostream>
#include <string>
#include "infoshare.h"
#include "infoshare.pb.h"
using namespace std;

Citbrains::infosharemodule::InfoShare info;

int main(int argc, char const *argv[])
{
    info.setup(Citbrains::Udpsocket::SocketMode::unicast_mode, 1, COLOR_MAGENTA, "127.0.0.1");

    CitbrainsMessage::SharingData proto;
    char i = 123;
    string s = {i};
    proto.set_id(s.c_str());
    proto.set_cf_own(s.c_str());
    proto.set_cf_ball(s.c_str());
    proto.set_status(s.c_str());
    proto.set_fps(s.c_str());
    proto.set_voltage(s.c_str());
    proto.set_temperature(s.c_str());
    proto.set_highest_servo(s.c_str());
    string command = "walking for ball";
    proto.set_command(command);
    string beh = "attacker now";
    proto.set_current_behavior_name(beh);
    string Bytes = proto.SerializeAsString();
    cout << "message size is " << Bytes.length() << endl;
    return 0;
}
