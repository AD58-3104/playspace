#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>
#include <bitset>
using namespace std;

int main(int argc, char const *argv[])
{
    using namespace std::literals::chrono_literals;
    char c = std::numeric_limits<unsigned char>::max();
    cout << bitset<8>(c) << endl;
    unsigned char uc = 200;
    // string s(static_cast<char>(64));
    string s{static_cast<char>(c)};
    uc = s[0];
    int i =  uc; 
    cout << bitset<8>(i) << "::" << i;

    return 0;
}
