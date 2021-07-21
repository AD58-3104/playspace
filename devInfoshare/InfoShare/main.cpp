#include <iostream>
#include <string>
#include "infoshare.pb.h"
using namespace std;
int main(int argc, char const *argv[])
{
    tutorial::Person proto;
    string pack;
    pack = string{12} + string{12} + string{23} + string{43}+ string{98}+ string{93}+ string{73}+ string{74};
    // proto.set_pack(pack);
    // proto.set_x(1024);
    // proto.set_id( string{12});
    // proto.set_cf_own(string{23});
    // proto.set_cf_ball(string{11});
    // proto.set_status(string{13});
    // proto.set_fps(string{14});
    // proto.set_voltage(string{15});
    // proto.set_temperature(string{16});
    // proto.set_highest_servo(string{17});
    string command = "walking for ball";
    proto.set_command(command);
    string beh = "attacker now";
    cout << command.length() << "::" << beh.length() << endl;
    proto.set_current_behavior_name(beh);
    string Bytes = proto.SerializeAsString();
    cout << sizeof(proto) << endl;
    cout << Bytes.length();
    return 0;
}
