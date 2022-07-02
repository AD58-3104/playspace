#include <future>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <boost/asio.hpp>
#include <atomic>
#include <memory>
#include <sstream>

void my_sleep(int64_t t)
{
    timespec target;
    timespec tmp;
    target.tv_sec = 0;
    target.tv_nsec = t;
    nanosleep(&target, &tmp);
}

const int64_t micro = 1000;
const int64_t mili = micro * 1000;
const int64_t sec = mili * 1000;
const int64_t max_loop = 1000;

using namespace std::literals::chrono_literals;
int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < max_loop; i++)
    {
        std::cout << " pool_time aaa us"
                  << "\n";
        // std::printf(" pool_time aaaa us\n");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    std::cout << "total time " << time << " us" << std::endl;
    std::cout << "total time " << time / 1000.0f << " ms" << std::endl;
    std::cout << "frame time " << time / static_cast<double>(max_loop) << " us" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < max_loop; i++)
    {
        std::stringstream ss;
        for (size_t j = 0; j < 10; j++)
        {
            ss << " pool_time  us" << j;
        }
        std::string str = ss.str();
    }
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << " us" << std::endl;
    return 0;
}