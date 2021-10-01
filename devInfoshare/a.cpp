#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{   
    char c = 64;
    // string s(static_cast<char>(64));
    string s{static_cast<char>(123)};
    cout << s << endl;
    return 0;
}
