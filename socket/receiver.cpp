#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <thread>
#include <cstring>
#include "picture.pb.h"
using namespace std;
using namespace chrono_literals;

int main(int argc, const char **argv)
{
    webotsvision::CameraMeasurement recv_pic;
    static const int32_t max_len_message = 800*480;
    std::string received_data;
    //received_data.resize(max_len);
    uint64_t recv_size = 0;
    unsigned int priority = 0;
    received_data.resize(max_len_message);
    boost::interprocess::message_queue::remove("WEBOTS_PICTURE_COMMUNICATION");
    boost::interprocess::message_queue msgq(boost::interprocess::create_only, "WEBOTS_PICTURE_COMMUNICATION", 5, max_len_message);
    while (1)
    {
        msgq.receive(&received_data[0], received_data.size(), recv_size, priority);
        std::cout << "recv_size is " << recv_size << std::endl;
        recv_pic.ParseFromString(received_data);
        //cout << recv_pic.DebugString() << endl;
        int x = recv_pic.height();
        cout << x << endl;
        std::this_thread::sleep_for(1000ms);
        //cout << recv_pic.width() << recv_pic.height() << recv_pic.simtime();
    }

    return 0;
}