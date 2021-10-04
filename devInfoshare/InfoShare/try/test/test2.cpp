// #define BOOST_TEST_MODULE TestUdpsocket
// #define BOOST_TEST_MAIN
#define BOOST_TEST_MAIN
#define BOOST_TEST_STATIC_LINK

#include "sUDPSocket.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/bind/bind.hpp>
#include <deque>
#include <chrono>
#include <memory>
#include <thread>
using namespace std::literals::chrono_literals;
using namespace Citbrains::Udpsocket;
using namespace boost::unit_test;

#define TEST_MESSAGE_SIZE 250


struct Socket_test
{
    const int32_t MESSAGE_SIZE;
    UDPClient client;
    UDPServer server;
    std::atomic_int32_t cnt;
    std::deque<std::string> dq;
    Socket_test(int32_t size, int32_t port) : MESSAGE_SIZE(size), client(std::string("127.0.0.1"),
                                                                               port, SocketMode::unicast_mode),
                                                    server(
                                                        port,
                                                        [&](std::string &&s) -> void
                                                        {
                                                            // std::cout << s << std::endl;
                                                            dq.emplace_back(std::move(s));
                                                            cnt++;
                                                            return;
                                                        },
                                                        SocketMode::unicast_mode),
                                                    cnt(0)
    {
    }
    ~Socket_test()
    {
        client.terminate();
        server.terminate();
    }
    void test_case1();
};

void Socket_test::test_case1()
{
    std::vector<std::string> vs;
    const int32_t message_size = MESSAGE_SIZE;
    const int32_t timeout_msec = message_size / 50 * 1000 /**/;
    for (int i = 0; i < message_size; ++i)
    {
        vs.push_back(std::string(160,'x') + std::to_string(i));
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
    for (int32_t loop_i = 0;; ++loop_i)
    {
        std::this_thread::sleep_for(10ms);
        if (loop_i > (timeout_msec / 10))
        {
            char str[256];
            sprintf(str, "test failed with timeout:: message_size is %d over %d msec", message_size, timeout_msec);
            BOOST_FAIL(str);
        }
        if (cnt >= message_size)
            break;
    }
    BOOST_REQUIRE_EQUAL_COLLECTIONS(vs.begin(), vs.end(), dq.begin(), dq.end());
    std::cout << "--------number of sended message is " << cnt << "  -----------\n";
    std::cout << "---------------timeout sec is " << (float)(MESSAGE_SIZE / 50) << "------------------" << std::endl;
}

void free_test(){
    BOOST_TEST(true);
}


test_suite *init_unit_test_suite(int /*argc*/, char * /*argv*/[])
{
    boost::unit_test::test_suite* master_test_suite = BOOST_TEST_SUITE( "TestUdpsocket" );
    // framework::master_test_suite().p_name.value = "TestUdpsocket";
    //^^^^^^^^^^^^^^^^^^^^^   setting  test condition ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    constexpr int32_t case_num = 6;
    constexpr int32_t init_port = 7777;
    constexpr int32_t message_size = 50;
    std::cout << "in init_unit_test_suite initializing...." << std::endl;
    //-------------------------register suites--------------------------------------------
    std::list<std::string> suite_names;
    for (int i = 1; i < case_num + 1; ++i)
    {
        using namespace std::literals::string_literals;
        suite_names.push_back("test_suite"s + std::to_string(i));
    }
    std::vector<test_suite *> suite_list;
    for (const auto &itr : suite_names)
        suite_list.push_back(BOOST_TEST_SUITE(itr.c_str()));
    //-----------------------------------------------------------------------------------
    //------------------------------add case---------------------------------------------
    std::vector<boost::shared_ptr<Socket_test>> test_list;
    for (int i = 0; i < case_num; ++i)
    {
        test_list.push_back(boost::make_shared<Socket_test>(message_size + 50 * i, init_port + i));
        char str[256];
        sprintf(str, "test-case-messagesize%d", message_size + 50 * i);
        // framework::master_test_suite().add(BOOST_TEST_CASE_NAME(boost::bind(&Socket_test::test_case1, test_list[i]),str));
        suite_list[i]->add(BOOST_TEST_CASE_NAME(boost::bind(&Socket_test::test_case1, test_list[i]),str));
        // suite_list[i]->add(BOOST_TEST_CASE(boost::bind(&Socket_test::test_case1, test_list[i])));
    }
    // test_suite *ts1 = BOOST_TEST_SUITE("test_suite1");
    // ts1->add(BOOST_TEST_CASE(boost::bind(&test_case1, 100), ));
    //-----------------------------------------------------------------------------------
    for (const auto &ts : suite_list)
    {
        master_test_suite->add(ts);
    }

    return master_test_suite;
}
