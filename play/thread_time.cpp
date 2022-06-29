#include <future>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/time.h>

void my_sleep(int64_t t){
    timespec target;
    timespec tmp;
    target.tv_sec = 0;
    target.tv_nsec = t;
    nanosleep(&target,&tmp);
}

const int32_t  micro = 1000;


using namespace std::literals::chrono_literals;
int main(int argc, char const *argv[])
{
    std::vector<int64_t> th_v;
    std::vector<int64_t> main_v;

    auto f = [&]() {
        auto start = std::chrono::high_resolution_clock::now();
        // std::this_thread::sleep_for(220us);
        my_sleep(micro * 220);
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast< std::chrono::microseconds >( diff ).count();
        std::cout << "thread time" << time << "\n";  
        // th_v.emplace_back(time);
    };
    for (size_t i = 0; i < 1000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto t = std::async(std::launch::async,f);
        auto t1 = std::async(std::launch::async,f);
        auto t2 = std::async(std::launch::async,f);
        auto t3 = std::async(std::launch::async,f);
        t.get();
        t1.get();
        t2.get();
        t3.get();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast< std::chrono::microseconds >( diff ).count();
        std::cout << "main loop time" << time << "\n";
        main_v.emplace_back(time);  
    }
    int64_t ave_time = 0;
    for(const auto&val :main_v){
        std::cout << "main_v :: " << val / 1000.0f<< " ms"<< std::endl;
        ave_time += val;
    }
    std::cout << "average main_v " <<ave_time / 1000.0f << " us  " << std::endl; 
    return 0;

}