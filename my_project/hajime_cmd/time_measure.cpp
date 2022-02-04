#include <string>
#include <optional>
#include <iostream>
#include <chrono>
using namespace std::literals;
std::optional<std::string> f()
{
    return std::string("aaaa");
}
std::string f2()
{
    return std::string("aaaa");
}
int main(int argc, char const *argv[])
{
    // std::optional<std::string> op;
    // if (op == std::nullopt)
    // {
    //     std::cout << "null !!!" << std::endl;
    // }
    // auto fl = f().has_value();
    // if (!fl)
    // {
    //     std::cout << "null\n";
    // }
    std::chrono::system_clock::time_point start, end;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < 10'000'000; ++i)
    {
        auto a = f();
        if (a.has_value()) //ほぼ何も無し
        {
        }
    }
    end = std::chrono::system_clock::now(); 
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "optional time " << elapsed << "\n";

    start = std::chrono::system_clock::now();
    for (int i = 0; i < 10'000'000; ++i)
    {
        auto a = f2();
    }
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "normal time " << elapsed << "\n";
    return 0;
}
//-O2を付ければ1000万回実行で大体85msと40msになった。毎秒どんなに多くても100回程度しか実行しないならオーバーヘッドは無視出来る範囲.