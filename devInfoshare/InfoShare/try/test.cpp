#define BOOST_TEST_MODULE TestUdpsocket

#include "sUDPSocket.hpp"
#include <boost/test/unit_test.hpp>
using namespace Citbrains::Udpsocket;

// BOOST_AUTO_TEST_SUITE(udpsocket_suite)

struct TestFixtureServer
{
    UDPServer server;
    std::atomic_int32_t cnt;
    TestFixtureServer() : server(
                              7880, [&](std::string &&s) -> void
                              {
                                  std::cout << s << std::endl;
                                  cnt++;
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

BOOST_FIXTURE_TEST_CASE(test_case1, TestFixtureClient)
{
    for (int i = 0; i < 1000; ++i)
    {
        std::string s = std::string("send times::") + std::to_string(i);
        client.send(std::move(s));
    }
}

BOOST_AUTO_TEST_CASE( test_case3 )
{
  BOOST_TEST( true );
}

// BOOST_AUTO_TEST_SUITE_END()