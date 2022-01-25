#include <iostream>
#include <vector>
#include <string>
using namespace std::literals;
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <future>
#include <chrono>
#include <thread>
#include <typeinfo>
#include <cxxabi.h>
#include <mutex>
#include <atomic>

template <class T>
void show_typename(T &&obj)
{
    int i;
    char *name = abi::__cxa_demangle(typeid(std::forward<T>(obj)).name(), 0, 0, &i);
    if (name != NULL)
    {
        if (i == 0)
            std::cout << "type is " << name << std::endl;
        free(name);
    }
    std::cout << __PRETTY_FUNCTION__ << "\n";
    return;
}

struct lcl
{
    lcl() : num(i)
    {
        std::cout << "construct lcl::" << i << std::endl;
        // num = i;
        i++;
    }
    ~lcl()
    {
        // std::cout << "destruct lcl::" << num << std::endl;
    }
    inline static int i = 0;
    int num;
};

struct test
{
    void f()
    {
        static thread_local lcl ins;
    }
};

void th_f()
{
    // static thread_local lcl ins;
    test a;
    for (int i = 0; i < 10; ++i)
    {
        a.f();
    }
    std::this_thread::sleep_for(1000ms);
}

int main(int argc, const char **argv)
{
    std::vector<std::future<void>> v;
    for (int i = 0; i < 10; i++)
    {
        v.emplace_back(std::async(std::launch::async, th_f));
        std::this_thread::sleep_for(50ms);
    }
    std::this_thread::sleep_for(500ms);
    std::cout << "-------------end----------------" << std::endl;
    return 0;
}
