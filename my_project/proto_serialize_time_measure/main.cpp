#include <iostream>
#include <chrono>
#include <vector>
#include "data.pb.h"
int main(int argc, char const *argv[])
{
    test::data d1;
    size_t loop = 100000;
    int64_t sum = 0;
    size_t test_cnt = 1000;
    for (int j = 0; j < test_cnt; ++j)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < loop; ++i)
        {
            d1.set_a("Miura Azusa");
            d1.set_b(40);
            d1.set_c(167);
            d1.set_d(999999);
            d1.set_e(1240000);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        int64_t tim = std::chrono::duration<int64_t, std::micro>(t).count();
        sum += tim;
    }
    std::cout << loop << "times serialize" << std::endl;
    std::cout << "average time " << (double)sum / (double)test_cnt / 1000.0 << "ms" << std::endl;
    std::cout << "1 serialize" << (double)sum / (double)test_cnt / loop << "μs"<< std::endl;
    return 0;
}
