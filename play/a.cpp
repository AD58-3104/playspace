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

void
f(string s)
{
    cout << "f is called string " << endl;
}
void f(const char *s)
{
    cout << "f is called const char*" << endl;
}

int main(int argc, const char **argv)
{
    string s("inooooo");
    f(s.c_str());
    st(s.c_str());
    cout << "-------------\n";
    f("inoue satoshi");
    f("inoue"s);
    st("aaaaa");
    st("aaa"s);
    return 0;
}