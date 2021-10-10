#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono_literals;
struct lazy{
    template<class F, class...A> 
    auto operator()(F&& f,A&&... a){
        return [&](auto&&... b){
            return std::forward<F>(f)(std::forward<A>(a)...,std::forward<decltype(b)>(b)...);
        };
    }
};

template<class F,typename... Args>
void measuringFuncTime(F&& f,Args&&... args){
    chrono::system_clock::time_point start,end;
    start = chrono::system_clock::now();
    f(std::forward<decltype(args)>(args)...);
    end = chrono::system_clock::now();
    double elapsed = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << elapsed << endl;
};

void longfunc(long long a,int j){
    this_thread::sleep_for(1000ms);
}

template<typename... T>
void hel(T... args){
    // std::cout << sizeof...(Args) << std::endl;
    int result = 0;
    result = (0 + ... + args);
    cout << result;
}

int main(int argc, const char** argv) {
    auto _ = lazy{};
    measuringFuncTime(longfunc,100000,123);
    return 0;
}