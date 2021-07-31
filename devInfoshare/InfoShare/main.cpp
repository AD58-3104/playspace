#include <iostream>
#include <string>
#include "infoshare.pb.h"
using namespace std;
int main(int argc, char const *argv[])
{
    CitbrainsMessage::SharingData proto;
    string pack;
    int i = 12;
    // pack = string{i} + string{12} + string{23} + string{43}+ string{98}+ string{93}+ string{73}+ string{74};
    // proto.set_pack(pack);
    // proto.set_x(1024);
    // proto.set_id( string{i});
    // proto.set_cf_own(string{i});
    // proto.set_cf_ball(string{i});
    // proto.set_status(string{i});
    // proto.set_fps(string{i});
    // proto.set_voltage(string{i});
    // proto.set_temperature(string{i});
    // proto.set_highest_servo(string{i});
    // string command = "walking for ball";
    // // proto.set_command(command);
    // string beh = "attacker now";
    // cout << command.length() << "::" << beh.length() << endl;
    // // proto.set_current_behavior_name(beh);
    // string Bytes = proto.SerializeAsString();
    // cout << sizeof(proto) << endl;
    // cout << Bytes.length();
    return 0;
}
