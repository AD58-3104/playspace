#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <thread>
#include <cstring>
using namespace std;
using namespace chrono_literals;
int main(int argc, const char **argv)
{
    constexpr int size = 320*240;
    std::string Data(size,'0');
    {
        boost::interprocess::message_queue msgq(boost::interprocess::open_only, "UNIQUE_KEY_HOGEHOGE");

        for (int i = 0; i < 3; ++i){
            msgq.send(&Data[0], Data.size(), 0);
            std::this_thread::sleep_for(1000ms);
            std::cout << Data.size() << std::endl;
        }
    }
    return 0;
}