#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace std::literals::string_literals;
int callback(int a)
{
    cout << "kansuu" << endl;
    return a * 2;
}

struct st
{
    int i = 0;
    st(string s)
    {
        cout << "st constructor called string " << endl;
    }
    st(const char *)
    {
        cout << "st constructor called const char*" << endl;
    }
};

void f(string s)
{
    cout << "f is called string " << endl;
}
void f(const char *s)
{
    cout << "f is called const char*" << endl;
}

int main(int argc, const char **argv)
{
    char c = 0;
    string s{c};
    
    int32_t i = s.at(0);
    cout << "size "<< s.size() << endl;
    cout << s << endl;
    cout << i << endl;
    return 0;
}