#include "header.hpp"
using namespace Citbrains::infosharemodule;

int main(int argc, char const *argv[])
{

    UDPClient client("127.0.0.1",7110,SocketMode::unicast_mode);

    
    for(int i = 0;i < 5;++i)
        client.send(std::string("ieeeeeeeeeeeeei") + std::to_string(i));
    std::string end("end");
    std::this_thread::sleep_for(4s);
    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!";
    return 0;
}
