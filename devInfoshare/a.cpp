#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
using namespace std;


void func(std::string&& s){
    string ss = std::move(s);
    cout << ss << endl;
}




int main(int argc, char const *argv[])
{
    string st("lambda");
    vector<std::unique_ptr<std::string>> vs;
    auto& vcon = vs;
    vcon.push_back(std::make_unique<std::string>("watashi da gahaha"));
    std::function<void(std::string&& s)> f = [](string&& s){
        func(std::forward<decltype(s)>(s));
    };
    
    cout << *(vcon[0]) << endl;
    return 0;
}



