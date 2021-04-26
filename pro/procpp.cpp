#include <iostream>
#include "pro.pb.h"
#include <fstream>
#include <string>



int main(int argc, char const *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    tutorial::person person;
    person.set_id(123);
    person.set_name("inoue satoshi");
    person.set_email("s19c1012");

    if(!stream.bad()){
        person.SerializeToOstream(&stream);
        stream.close();
    }
    std::cout << "done" << std::endl;
    return 0;
}
