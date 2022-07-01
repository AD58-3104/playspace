#include <future>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <boost/asio.hpp>
#include <atomic>
#include <memory>

struct threadPoolRAII{
    boost::asio::thread_pool pools_;
    boost::asio::executor_work_guard<boost::asio::thread_pool::executor_type> work_;
    threadPoolRAII(const size_t& thread_num):pools_(thread_num),work_( boost::asio::make_work_guard(pools_.get_executor())){
    }
    ~threadPoolRAII(){
        work_.reset();
        pools_.stop();
    }
};

threadPoolRAII th_pool(4);
inline std::atomic_int_fast32_t countor(0);

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

using namespace std::literals::chrono_literals;
int main(int argc, char const *argv[])
{
    std::vector<int64_t> th_v;
    std::vector<int64_t> async_v;
    std::vector<int64_t> thread_v;
    std::vector<int64_t> thread_pool_v;
    // auto pool_wait_th = std::thread([&](){
    //         th_pool.pools_.wait();
    //         th_pool.pools_.join();
    // });

    auto f = [&](std::promise<bool>& pro)
    {
        auto start = std::chrono::high_resolution_clock::now();
        // std::this_thread::sleep_for(220us);
        my_sleep(micro * 220);
        // my_sleep(220);
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        countor++;
        pro.set_value(true);
        // th_v.emplace_back(time);
    };
    auto nomal_f = [&]()
    {
        auto start = std::chrono::high_resolution_clock::now();
        // std::this_thread::sleep_for(220us);
        my_sleep(micro * 220);
        // my_sleep(220);
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        countor++;
        // th_v.emplace_back(time);
    };
    for (size_t i = 0; i < 1000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto t = std::async(std::launch::async, nomal_f);
        auto t1 = std::async(std::launch::async, nomal_f);
        auto t2 = std::async(std::launch::async, nomal_f);
        auto t3 = std::async(std::launch::async, nomal_f);
        t.get();
        t1.get();
        t2.get();
        t3.get();
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        // std::cout << "main loop time" << time << "\n";
        async_v.emplace_back(time);
        countor++;
    }
    for (size_t i = 0; i < 1000; i++)
    {
        std::promise<bool> last_p1;
        std::promise<bool> last_p2;
        std::promise<bool> last_p3;
        std::promise<bool> last_p4;
        std::future<bool> last_f1 = last_p1.get_future();
        std::future<bool> last_f2 = last_p2.get_future();
        std::future<bool> last_f3 = last_p3.get_future();
        std::future<bool> last_f4 = last_p4.get_future();
        auto start = std::chrono::high_resolution_clock::now();
        auto t = std::thread(std::bind(f,std::ref(last_p1)));
        auto t1 = std::thread(std::bind(f,std::ref(last_p2)));
        auto t2 = std::thread(std::bind(f,std::ref(last_p3)));
        auto t3 = std::thread(std::bind(f,std::ref(last_p4)));
        t.join();
        t1.join();
        t2.join();
        t3.join();
        bool ok = last_f1.get();
        ok = last_f2.get() && ok;
        ok = last_f3.get() && ok;
        ok = last_f4.get() && ok;
        if(ok){
            countor++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        // std::cout << "main loop time " << time << " us" << "\n";
        thread_v.emplace_back(time);
    }
    for (size_t i = 0; i < 1000; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::promise<bool> last_p1;
        std::promise<bool> last_p2;
        std::promise<bool> last_p3;
        std::promise<bool> last_p4;
        std::future<bool> last_f1 = last_p1.get_future();
        std::future<bool> last_f2 = last_p2.get_future();
        std::future<bool> last_f3 = last_p3.get_future();
        std::future<bool> last_f4 = last_p4.get_future();
        boost::asio::post(th_pool.pools_,std::bind(f,std::ref(last_p1)));
        boost::asio::post(th_pool.pools_,std::bind(f,std::ref(last_p2)));
        boost::asio::post(th_pool.pools_,std::bind(f,std::ref(last_p3)));
        boost::asio::post(th_pool.pools_,std::bind(f,std::ref(last_p4)));
        // th_pool.pools_.wait();
        bool ok = last_f1.get();
        ok = last_f2.get() && ok;
        ok = last_f3.get() && ok;
        ok = last_f4.get() && ok;
        if(ok){
            countor++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        int64_t time = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
        // std::cout << " pool_time " << time << " us" << "\n";
        thread_pool_v.emplace_back(time);

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
    ave_time = 0;
    for (const auto &val : thread_pool_v)
    {
        std::cout << "thread_pool_v :: " << val / 1000.0f << " ms" << std::endl;
        ave_time += val;
    }
    std::cout << "average thread_pool_v " << ave_time / 1000.0f << " us  " << std::endl;
    std::cout << "countor " << countor.load() << std::endl;
    return 0;
}