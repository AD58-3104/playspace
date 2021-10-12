#include "sUDPSocket.hpp"
using namespace Citbrains::Udpsocket;
using namespace std;
#include "infoshare.pb.h"

int main(int argc, char const *argv[])
{

    UDPServer server(7110,[](std::string&& s)->void{
        CitbrainsMessage::SharingData data;
        data.ParseFromString(std::string(std::move(s)));
        std::cout << data.DebugString() << std::endl;
        return;
    }, SocketMode::unicast_mode);

    std::this_thread::sleep_for(3000ms);
    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    return 0;
}
