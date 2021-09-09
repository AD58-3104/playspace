#include <string>
#include <iostream>
#include <functional>
using namespace std;


void func(std::string&& s){
    string ss = std::move(s);
    cout << ss << endl;
}




int main(int argc, char const *argv[])
{
    string st("lambda");
    std::function<void(std::string&& s)> f = [](string&& s){
        func(std::forward<decltype(s)>(s));
    };
    string ino("inoue satoshi");
    f(move(ino));
    // func(std::move(ino));
    cout <<"moved"<< ino ;
    return 0;
}



