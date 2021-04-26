#include <iostream>
#include "pro.pb.h"
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    tutorial::Person person;
    person.set_id(123);
    person.set_name("inoue satoshi");
    person.set_email("s19c1012");
    fstream output("../stream.txt", ios::out | ios::trunc | ios::binary);
    for (size_t i = 0; i < 10; i++)
    {
        if (!output.bad())
        {
            person.SerializeToOstream(&output);
            output.close();
        }
        person.set_id(i);
    }

    std::cout << "done" << std::endl;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
