#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

using namespace std;

// キュー作成。識別用として、固有のキーを指定します
struct MES{
    char * str;
    MES();
};

int main(int argc, const char **argv)
{
    constexpr int size = 640*480;
    //このサイズのstringのコピーは164microsecだった。まあ遅いけど大して問題にはならないかも。
    std::string message;
    message.resize(size);
    uint64_t recv_size = 0;
    unsigned priority = 0;
    boost::interprocess::message_queue::remove("UNIQUE_KEY_HOGEHOGE");
    boost::interprocess::message_queue msgq(boost::interprocess::create_only, "UNIQUE_KEY_HOGEHOGE", 5, size);
    while (1)
    {
        msgq.receive(&message[0], message.size(), recv_size, priority);
        std::cout << "recv_size is" << recv_size << std::endl;
    }
    return 0;
}