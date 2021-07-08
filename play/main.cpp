#include <iostream>
#include <string>
#include "infoshare.pb.h"
using namespace std;
int main(int argc, char const *argv[])
{
    tutorial::Person proto;
    proto.set_x(112);
    string Bytes = proto.SerializeAsString();
    cout << sizeof(proto) << endl;
    cout << Bytes.length();
    return 0;
}
