#include <iostream>
#include <chrono>
#include <type_traits>
#include <functional>
#include <iostream>
#include <string>
using namespace std;
using namespace chrono_literals;

class c{
    public:
    int a = 1;
    void f(){
        cout << "call" << endl;
        a++;
        if(a == 10)return;
        f();
    }
};

using type_i = string;

void ff(type_i arg){
    cout << "call ff";
}

int main(int argc, const char **argv)
{
    c ins;
    ins.f();
    int i = 12;
    ff(i);
    return 0;
}