#include "sAsyncSocket.hpp"
#include <vector>
using namespace Citbrains::Asyncsocket;

int main(int argc, char const *argv[])
{
    boost::asio::io_service io_service;
    std::vector<std::string> stringContainer;
    TCPServer server1(
        7110, [&stringContainer](std::string &&s)
        {
            std::string moved_string(std::move(s));
            std::cout << moved_string;
            stringContainer.push_back(moved_string);
            std::cout << "server1::message -->" << moved_string << std::endl;
        },
        SocketMode::unicast_mode);
    std::cout << "server created \n";
    std::this_thread::sleep_for(20s);
    server1.terminate();
    std::cout << "terminated ";
    std::this_thread::sleep_for(3s);
    for (const auto &itr : stringContainer)
        std::cout << itr << std::endl;
    return 0;
}
