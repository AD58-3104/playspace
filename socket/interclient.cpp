#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <thread>
#include <cstring>
using namespace std;
using namespace chrono_literals;
int main(int argc, const char **argv)
{
    int size = 320*240;
    unsigned char Data[size];
    memset(Data,0,size);
    int end = 1;
    {
        //boost::interprocess::message_queue::remove("UNIQUE_KEY_HOGEHOGE");
    } {
        std::cout << sizeof(Data) << std::endl; 
        boost::interprocess::message_queue msgq(boost::interprocess::open_only, "UNIQUE_KEY_HOGEHOGE");
        for (int i = 0; i < 10; ++i){
            if(i == 0)msgq.send(&Data, sizeof(Data), 0);
            std::this_thread::sleep_for(1000ms);
            std::cout << "start" << std::endl;
        }
    }
    return 0;
}