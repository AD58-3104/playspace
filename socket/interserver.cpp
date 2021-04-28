#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

using namespace std;

// キュー作成。識別用として、固有のキーを指定します

int main(int argc, const char **argv)
{
    int size = 320*240;
    unsigned char* message = (unsigned char*)malloc(size);
    uint64_t recv_size = size;
    unsigned priority = 0;
    boost::interprocess::message_queue::remove("UNIQUE_KEY_HOGEHOGE");
    boost::interprocess::message_queue msgq(boost::interprocess::create_only, "UNIQUE_KEY_HOGEHOGE", 2, 640*480);
    int cnt = 0;
    while (1)
    {
        msgq.receive(&message, size*2, recv_size, priority);
        std::cout << "cnt is" << cnt << std::endl;
        cnt++;
    }
    return 0;
}