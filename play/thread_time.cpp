#include <future>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/time.h>

void my_sleep(int64_t t)
{
    timespec target;
    timespec tmp;
    target.tv_sec = 0;
    target.tv_nsec = t;
    nanosleep(&target, &tmp);
}

const int32_t micro = 1000;

using namespace std::literals::chrono_literals;
int main(int argc, char const *argv[])
{
    std::vector<int64_t> th_v;
    std::vector<int64_t> async_v;
    std::vector<int64_t> thread_v;

    auto f = [&]()
    {
        auto start = std::chrono::high_resolution_clock::now();
        // std::this_thread::sleep_for(220us);
        my_sleep(micro * 220);
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        // std::cout << "thread time" << time << "\n";
        // th_v.emplace_back(time);
    };
    for (size_t i = 0; i < 1000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto t = std::async(std::launch::async, f);
        auto t1 = std::async(std::launch::async, f);
        auto t2 = std::async(std::launch::async, f);
        auto t3 = std::async(std::launch::async, f);
        t.get();
        t1.get();
        t2.get();
        t3.get();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        std::cout << "main loop time" << time << "\n";
        async_v.emplace_back(time);
    }
    for (size_t i = 0; i < 1000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto t = std::thread(f);
        auto t1 = std::thread(f);
        auto t2 = std::thread(f);
        auto t3 = std::thread(f);
        t.join();
        t1.join();
        t2.join();
        t3.join();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        std::cout << "main loop time" << time << "\n";
        thread_v.emplace_back(time);
    }
    int64_t ave_time = 0;
    for (const auto &val : async_v)
    {
        std::cout << "async_v :: " << val / 1000.0f << " ms" << std::endl;
        ave_time += val;
    }
    std::cout << "average async_v " << ave_time / 1000.0f << " us  " << std::endl;
    ave_time = 0;
    for (const auto &val : thread_v)
    {
        std::cout << "thread_v :: " << val / 1000.0f << " ms" << std::endl;
        ave_time += val;
    }
    std::cout << "average thread_v " << ave_time / 1000.0f << " us  " << std::endl;
    return 0;
}