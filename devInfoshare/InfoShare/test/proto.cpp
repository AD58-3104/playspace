#include <iostream>
#include "infoshare.pb.h"
#include <string>

int main(int argc, char const *argv[])
{
    CitbrainsMessage::Pos2D pos2d;
    pos2d.set_pos_x(123);
    pos2d.set_pos_y(123);
    pos2d.set_pos_theta(123);
    std::string size = pos2d.SerializeAsString();
    std::cout << "------pos2d size is " << size.size() << "----- " << std::endl;
    CitbrainsMessage::Pos2DCf pos2dcf;
    pos2dcf.mutable_position()->set_pos_x(123);
    pos2dcf.mutable_position()->set_pos_y(123);
    pos2dcf.mutable_position()->set_pos_theta(123);
    // pos2dcf.set_is_detect(true);
    CitbrainsMessage::SharingData da;
    std::string mozi;
    for(int i = 0;i < 100;++i)
        mozi.push_back((char)('a'));
    da.set_command_st(mozi);
    std::cout << "string is " << mozi << "   size is " << mozi.size() <<  std::endl;
    std::cout << da.SerializeAsString().size() << " <-da size" << std::endl;
    pos2dcf.set_confidence(std::string{static_cast<char>(96)});
    std::string size2 = pos2dcf.SerializeAsString();
    std::cout << "------pos2d size is " << size2.size() << "----- " << std::endl;
    CitbrainsMessage::SharingData sharingdata;

    return 0;
}
