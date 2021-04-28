#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <thread>
#include <cstring>
#include "picture.pb.h"
using namespace std;
using namespace chrono_literals;


int main(int argc, const char **argv)
{
    webotsvision::CameraMeasurement picture,t;
    constexpr int size = 640*480;
    std::string image(size,'0');
    std::string send_data;
    picture.set_image(image);
    picture.set_simtime(765765);
    picture.set_width(640);
    picture.set_height(480);
    send_data = picture.SerializeAsString();
    //t.ParseFromString(send_data);
    //cout << t.DebugString();
    //cout << t.simtime() << endl;
    {
        boost::interprocess::message_queue msgq(boost::interprocess::open_only, "WEBOTS_PICTURE_COMMUNICATION");

        for (int i = 0; i < 10; ++i){
            msgq.send(&send_data[0], send_data.size(), 0);
            std::this_thread::sleep_for(1000ms);
            std::cout << send_data.size() << std::endl;
        }
    }
    return 0;
}