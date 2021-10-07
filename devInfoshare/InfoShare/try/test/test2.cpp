// #define BOOST_TEST_MODULE TestUdpsocket
// #define BOOST_TEST_MAIN
// #define BOOST_TEST_MAIN
// #define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "sUDPSocket.hpp"
#include <boost/bind/bind.hpp>
#include <deque>
#include <chrono>
#include <memory>
#include <thread>
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace Citbrains::Udpsocket;
using namespace boost::unit_test;

struct Socket_test
{
    const int32_t SendMessage_num;
    static const int32_t message_length = 256;
    //------------notify--------------
    std::mutex notify_mut;
    bool finish;
    std::condition_variable cv;
    //--------------------------------
    UDPClient client;
    UDPServer server;
    std::atomic_int32_t cnt = 0;
    std::set<std::string> dq;
    int32_t Port;

    Socket_test(int32_t size, int32_t port) : SendMessage_num(size), finish(false), client(std::string("127.0.0.1"),
                                                                                           port, SocketMode::unicast_mode),
                                              server(
                                                  port,
                                                  [&](std::string &&s) -> void
                                                  {
                                                      // std::cout << s << std::endl;
                                                      dq.insert(s);
                                                      cnt++;
                                                      if ((cnt >= SendMessage_num) || (s == "end"s))
                                                      {
                                                          dq.erase("end");
                                                          if (s == "end"s)
                                                              cnt--;
                                                          std::lock_guard lk(notify_mut);
                                                          finish = true;
                                                          cv.notify_all();
                                                      }
                                                      return;
                                                  },
                                                  SocketMode::unicast_mode),
                                              cnt(0), Port(port)
    {
    }
    ~Socket_test()
    {
        client.terminate();
        server.terminate();
    }
    void teardown()
    {
        client.terminate();
        server.terminate();
    }
    void test_case1();
};

void Socket_test::test_case1()
{
    std::set<std::string> vs;
    const int32_t message_size = SendMessage_num;
    const int32_t timeout_msec = message_size / 50 * 1000;
    using namespace std::literals::string_literals;
    for (int i = 0; i < message_size; ++i)
    {
        vs.insert(std::to_string(i) + std::string('a', message_length));
    }
    auto th = std::thread(
        [&]()
        {
            for (int i = 0; i < 2; ++i)
            {
                for (auto s : vs)
                {
                    std::this_thread::sleep_for(100ns);
                    client.send(std::move(std::string(s)));
                }
            }
            std::this_thread::sleep_for(2000ms); //timeout
            for (int i = 0; i < 50; i++)
            {
                std::this_thread::sleep_for(10ms);
                client.send(std::string("end"));
            }
        });
    th.detach();
    std::cout << "------------start send------------" << std::endl;
    {
        std::unique_lock<std::mutex> lock(notify_mut);
        cv.wait(lock, [&]
                { return finish; });
    }
    if (cnt < message_size)
    {
        char str[256];
        std::cout << "--------loss is " << cnt - SendMessage_num << "  -----------\n";
        sprintf(str, "test failed with timeout:: message_size is %d over 2 sec", message_size);
        BOOST_FAIL(str);
    }

    BOOST_REQUIRE_EQUAL_COLLECTIONS(vs.begin(), vs.end(), dq.begin(), dq.end());
    std::cout << "--------port " << Port << " number of sended message is " << cnt << "  -----------\n";
    std::cout << "--------loss is " << cnt - SendMessage_num << "  -----------\n";

    // teardown();
}

bool init_unit_test_suite(/*int argc, char * argv[]*/)
{
    framework::master_test_suite().p_name.value = "TestUdpsocket";
    //^^^^^^^^^^^^^^^^^^^^^   setting  test condition ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    constexpr int32_t case_num =20;
    constexpr int32_t init_port = 7777;
    constexpr int32_t message_size = 1000;
    std::cout << "in init_unit_test_suite initializing...." << std::endl;
    //-------------------------register suites--------------------------------------------
    std::list<std::string> suite_names;
    for (int i = 0; i < case_num; ++i)
    {
        using namespace std::literals::string_literals;
        suite_names.push_back("test_suite"s + std::to_string(i));
    }
    std::vector<test_suite *> suite_list;
    for (const auto &itr : suite_names)
        suite_list.push_back(BOOST_TEST_SUITE(itr.c_str()));
    //------------------------------add case---------------------------------------------
    std::vector<boost::shared_ptr<Socket_test>> test_list;
    std::vector<std::string> test_case_name_list;
    for (int i = 0; i < case_num; ++i)
    {
        test_list.push_back(boost::make_shared<Socket_test>(message_size + 50 * i, init_port + i));
        char str[256];
        sprintf(str, "test-case-messagesize%d", message_size + 20 * i);
        test_case_name_list.emplace_back(str);
    }
    for (int i = 0; i < case_num; ++i)
    {
        // for (int j = 0; j < case_num; ++j)
        suite_list[i]->add(BOOST_TEST_CASE_NAME(boost::bind(&Socket_test::test_case1, test_list[i]), test_case_name_list[i].c_str()), 0, 2);
    }
    //-----------------------------------------------------------------------------------
    for (const auto &ts : suite_list)
    {
        framework::master_test_suite().add(ts);
    }

    return true;
}
bool init_unit_test()
{
    init_unit_test_suite();
    return true;
}

int main(int argc, char *argv[])
{
    return boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
