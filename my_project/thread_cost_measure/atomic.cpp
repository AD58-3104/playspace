#include <thread>
#include <iostream>
#include <atomic>

static inline std::atomic_int_fast32_t countor(0);

int main(int argc, const char** argv) {
    auto f = [&](){
        for (size_t i = 0; i < 1000000; i++)
        {
            countor++;
        }
        std::cout << "thread -alalalala\n";
    };
    for(int i = 0; i < 200;++i){
        std::thread t1(f);
        std::thread t2(f);
        t1.join();
        t2.join();
    }
    std::cout << countor << std::endl;
    return 0;
}