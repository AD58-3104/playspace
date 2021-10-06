
#include "sUDPSocket.hpp"
#include <deque>
#include <chrono>
#include <memory>
#include <thread>
using namespace std::literals::chrono_literals;
using namespace Citbrains::Udpsocket;

#define TEST_MESSAGE_SIZE 250

struct Socket_test
{
    const int32_t SendMessage_num;
    static const int32_t message_length = 512;
    //------------notify--------------
    std::mutex notify_mut;
    bool finish;
    std::condition_variable cv;
    //--------------------------------
    UDPClient client;
    UDPServer server;
    std::atomic_int32_t cnt;
    std::deque<std::string> dq;

    Socket_test(int32_t size, int32_t port) : SendMessage_num(size), finish(false), client(std::string("127.0.0.1"),
                                                                                           port, SocketMode::unicast_mode),
                                              server(
                                                  port,
                                                  [&](std::string &&s) -> void
                                                  {
                                                       std::cout << s << std::endl;
                                                      dq.emplace_back(std::move(s));
                                                      cnt++;
                                                      if (cnt >= SendMessage_num)
                                                      {
                                                          std::lock_guard lk(notify_mut);
                                                          finish = true;
                                                          cv.notify_all();
                                                      }
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
    const int32_t message_size = SendMessage_num;
    const int32_t timeout_msec = message_size / 50 * 1000;
    using namespace std::literals::string_literals;
    for (int i = 0; i < message_size; ++i)
    {
        // vs.push_back(std::to_string(i) + std::string('#', message_size));
        vs.push_back(std::to_string(i));
    }
    auto th = std::thread(
        [&]()
        {
            std::chrono::system_clock::time_point start, end; // 型は auto で可
            start = std::chrono::system_clock::now();         // 計測開始時間
            for (auto s : vs)
            {
                client.send(std::move(s));
            }
            end = std::chrono::system_clock::now();                                                      // 計測終了時間
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //処理に要した時間をミリ秒に変換
            std::cout << "send time elapsed is " << elapsed << "ms " << std::endl;
        });
    th.detach();
    std::chrono::system_clock::time_point start, end; // 型は auto で可
    start = std::chrono::system_clock::now();         // 計測開始時間

    {
        std::unique_lock<std::mutex> lock(notify_mut);
        cv.wait(lock, [&]
                { return finish; });
    }
    end = std::chrono::system_clock::now();                                                      // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //処理に要した時間をミリ秒に変換
    if (!finish)
    {
        char str[256];
        sprintf(str, "test failed with timeout:: message_size is %d over %d msec", message_size, timeout_msec);
    }

    // BOOST_REQUIRE_EQUAL_COLLECTIONS(vs.begin(), vs.end(), dq.begin(), dq.end());
    std::cout << "--------number of sended message is " << cnt << "  -----------\n";
    std::cout << "---------------timeout sec is " << (float)(SendMessage_num / 50) << "------------------" << std::endl;
    std::cout << "actualtime is :: " << elapsed << " || The 1 packet time is :: " << elapsed / message_size << "ms ||"
              << " through put is" << (message_size * message_length) / (elapsed) << "Kbyte/s" << std::endl;
}

int main(int argc, char *argv[])
{
    int message_num = 400;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Socket_test test(400, 7777);
    test.test_case1();
}