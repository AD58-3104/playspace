#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::literals::chrono_literals;

int main(int argc, const char** argv) {
    // short a = 0b01010101 01010101;
    short a = 0b0101010101010101;
    char c[2] = { 0b01010101,0b01010101};
    cout << a << endl;
    cout << c[0] << " " << c[1];
    cout << ((c[1] << 8) | c[0]);
    return 0;
}