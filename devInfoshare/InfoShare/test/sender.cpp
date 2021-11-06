#include "sUDPSocket.hpp"
using namespace Citbrains::Udpsocket;
using namespace std;
#include "infoshare.pb.h"

int main(int argc, char const *argv[])
{

    UDPClient client("127.0.0.1", 7120, SocketMode::broadcast_mode);
    for (int j = 99; j >= 5; --j)
    {
        CitbrainsMessage::SharingData proto;
        char i = static_cast<char>((unsigned char)j);
        std::string s = {i};
        char c = 2;
        proto.set_id(std::string{c}.c_str());
        char ucc = 5;
        proto.set_team_color(std::string{(char)ucc}.c_str());
        proto.set_cf_own(s.c_str());
        proto.set_cf_ball(s.c_str());
        proto.set_fps(s.c_str());
        proto.set_voltage(s.c_str());
        proto.set_temperature(s.c_str());
        proto.set_highest_servo(s.c_str());
        //-------------------辞書文字列 1,2を送る
        unsigned char cu = 1;
        std::string st;
        st.push_back(char(cu));
        st.push_back(char(cu+1));
        proto.set_command(st);
        std::cout << st << "st no nakami\n";
        proto.set_current_behavior_name(st);
        //-------------------
        proto.set_is_detect_ball(true);

        CitbrainsMessage::Pos2D pos2d;
        CitbrainsMessage::Pos2DCf pos2dcf;
        pos2d.set_pos_x(765);
        pos2d.set_pos_y(346);
        pos2d.set_pos_theta(9);

        pos2dcf.set_is_detect(true);
        pos2dcf.set_confidence(std::string{(char)77}.c_str());

        pos2dcf.mutable_position()->CopyFrom(pos2d);
        proto.add_our_robot_gl()->CopyFrom(pos2d);
        proto.add_enemy_robot_gl()->CopyFrom(pos2d);
        proto.add_black_pole_gl()->CopyFrom(pos2d);
        proto.add_target_pos_vec()->CopyFrom(pos2d);
        proto.mutable_ball_gl_cf()->CopyFrom(pos2dcf);
        proto.mutable_self_pos_cf()->CopyFrom(pos2dcf);


        s = proto.SerializeAsString();
        client.send(std::move(s));
        std::this_thread::sleep_for(1ms);
    }
    std::this_thread::sleep_for(1500ms);
    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    return 0;
}
