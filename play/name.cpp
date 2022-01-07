#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std::literals::string_literals;
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <typeinfo>
#include <cxxabi.h>
#include <type_traits>

// template <class T>
// void show_typename(T &obj)
// {
//     // char *name = abi::__cxa_demangle(typeid(std::forward<T>(obj)).name(), 0, 0, &i);
//     // if (name != NULL)
//     // {
//     //     if (i == 0)
//     //         std::cout << "type is " <<name << std::endl;
//     //     free(name);
//     // }
//     std::string s(__PRETTY_FUNCTION__);
//     auto i = s.find_first_of("[");
//     std::cout << "type is lvalue ref [" << s.substr(i + 5) << "\n";
//     return;
// }

template <class T>
void show_typename(T &&obj)
{
    // char *name = abi::__cxa_demangle(typeid(std::forward<T>(obj)).name(), 0, 0, &i);
    // if (name != NULL)
    // {
    //     if (i == 0)
    //         std::cout << "type is " <<name << std::endl;
    //     free(name);
    // }
    std::string s(__PRETTY_FUNCTION__);
    auto i = s.find_first_of("[");
    if (std::is_lvalue_reference<decltype(std::forward<T>(obj))>::value)
    {
        std::cout << "type is lvalue ref";
    }
    else if (std::is_rvalue_reference<decltype(std::forward<T>(obj))>::value)
    {
        std::cout << "type is rvalue ref";
    }
    else{
        std::cout << "type is " ;
    }
    std::cout << "[" << s.substr(i + 5) << "\n";
    return;
}

int main(int argc, const char **argv)
{
    const int &x = 0;
    decltype(auto) y = x;
    int i = 4;
    show_typename(x);
    show_typename(std::move(i));
    std::string s = "aaaaa";
    std::string ss = "ooo";
    swap(ss, s);
    return 0;
}
