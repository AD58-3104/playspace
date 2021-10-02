#define BOOST_TEST_MODULE TestUdpsocket

#include "sUDPSocket.hpp"
#include <boost/test/unit_test.hpp>
#include <deque>
#include <chrono>
#include <thread>
using namespace std::literals::chrono_literals;
using namespace Citbrains::Udpsocket;

BOOST_AUTO_TEST_SUITE(udpsocket_suite)

#define TEST_MESSAGE_SIZE 250
struct TestFixtureServer
{
    UDPServer server;
    std::atomic_int32_t cnt;
    std::deque<std::string> dq;
    TestFixtureServer() : server(
                              7880, [&](std::string &&s) -> void
                              {
                                  //   std::cout << s << std::endl;
                                  dq.emplace_back(std::move(s));
                                  return;
                              },
                              SocketMode::unicast_mode),
                          cnt(0)
    {
    }
    ~TestFixtureServer()
    {
        server.terminate();
    }
};

struct TestFixtureClient
{
    UDPClient client;
    TestFixtureClient() : client(std::string("127.0.0.1"), 7880, SocketMode::unicast_mode)
    {
    }
    ~TestFixtureClient()
    {
        client.terminate();
    }
};

struct TestFixtureSocket
{
    const int32_t MESSAGE_SIZE;
    UDPClient client;
    UDPServer server;
    std::atomic_int32_t cnt;
    std::deque<std::string> dq;
    TestFixtureSocket(int32_t size) :MESSAGE_SIZE(size), client(std::string("127.0.0.1"), 
    7880, SocketMode::unicast_mode), 
    server(
        7880,
        [&](std::string &&s) -> void
        {
            // std::cout << s << std::endl;
            dq.emplace_back(std::move(s));
            cnt++;
            return;
        },
        SocketMode::unicast_mode),cnt(0)
    {
    }
    ~TestFixtureSocket()
    {
        client.terminate();
        server.terminate();
    }
};

BOOST_FIXTURE_TEST_CASE(test_case1, TestFixtureSocket,* boost::unit_test::timeout(TEST_MESSAGE_SIZE / 50))
{
    std::vector<std::string> vs;
    const int32_t message_size = MESSAGE_SIZE;
    for (int i = 0; i < message_size; ++i)
    {
        vs.push_back(std::string("send times::") + std::to_string(i));
    }
    auto th = std::thread(
        [&]()
        {
            for (auto s : vs)
            {
                client.send(std::move(s));
            }
        });
    th.detach();

    while (true)
    {
        std::this_thread::sleep_for(10ms);
        if (cnt >= message_size)
            break;
    }
    BOOST_REQUIRE_EQUAL_COLLECTIONS(vs.begin(),vs.end(),dq.begin(),dq.end());
    std::cout << "--------number of sended message is " << cnt << "  -----------\n";
    std::cout << "---------------timeout sec is " << (float)(MESSAGE_SIZE / 50) << "------------------" << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()