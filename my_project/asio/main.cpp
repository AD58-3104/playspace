#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <future>
using namespace std::literals;
boost::asio::io_context ctx;

void heavytask()
{
    std::this_thread::sleep_for(1000ms);
    boost::asio::post(ctx, [&]()
                      { std::cout << "heavy task finish" << std::endl; });
}

void lighttask()
{
    std::this_thread::sleep_for(10ms);
    boost::asio::post(ctx, [&]()
                      { std::cout << "light task finish" << std::endl; });
}

class test
{
public:
    test() {}
    ~test() {}
};

int main(int argc, char const *argv[])
{
    std::vector<std::future<void>> ths;
    auto work = boost::asio::require(ctx.get_executor(), boost::asio::execution::outstanding_work.tracked);
    ths.emplace_back(std::async(std::launch::async, lighttask));
    ths.emplace_back(std::async(std::launch::async, heavytask));
    for (int i = 0; i < 10; ++i)
        ths.emplace_back(std::async(std::launch::async, lighttask));
    std::this_thread::sleep_for(10ms);
    for (int i = 0;i < 1000; i++)
    {
        // ctx.poll();
        if (i > 1000)
        {
            // ctx.stop();
        }
    }
    ctx.run();
    std::cout << "end" << std::endl;
    return 0;
}
