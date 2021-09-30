#include "header.hpp"
#include <vector>
using namespace Citbrains::Udpsocket;

int main(int argc, char const *argv[])
{
    boost::asio::io_service io_service;
    std::vector<std::string> stringContainer;
    UDPServer server1(
        7110, [&stringContainer](std::string &&s)
        {
            std::string moved_string(std::move(s));
            std::cout << moved_string;
            stringContainer.push_back(moved_string);
            std::cout << "server1::message -->" << moved_string << std::endl;
        },
        SocketMode::multicast_mode);
    UDPServer server2(
        7111, [](std::string &&s)
        {
            std::string moved_string(std::move(s));
            std::cout << "server2::message -->" << moved_string << std::endl;
        },
        SocketMode::multicast_mode);
    UDPServer server3(
        7112, [](std::string &&s)
        {
            std::string moved_string(std::move(s));
            std::cout << "server3::message -->" << moved_string << std::endl;
        },
        SocketMode::multicast_mode);
    std::cout << "server created \n";
    std::this_thread::sleep_for(20s);
    server1.terminate();
    server2.terminate();
    server3.terminate();
    // std::cout << "terminated ";
    std::this_thread::sleep_for(3s);
    // for (const auto &itr : stringContainer)
        // std::cout << itr << std::endl;
    return 0;
}
