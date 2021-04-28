#include <iostream>
#include "pro.pb.h"
#include <fstream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char const *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    tutorial::Person person;
    person.set_id(123);
    person.set_name("inoue satoshi");
    person.set_email("s19c1012");
    fstream output("stream.dat", ios::out | ios::trunc | ios::binary);
    std::fstream clearstream("null.dat", ios::in  | ios::binary);
    output << "int" ;
    int i = 0;
    for ( i = 0; i < 10; i++)
    {
        if (!output.bad())
        {
            output.open("stream.dat", ios::out | ios::trunc | ios::binary);
            //output << clearstream.rdbuf();
            person.SerializeToOstream(&output);
            std::cout << "serialize" << std::endl;
            //this_thread::sleep_for(60ms);
            output.close();
        }
        std::this_thread::sleep_for(1500ms);
        person.clear_id();
        person.clear_name();
        person.clear_email();
        person.set_name("inoue satoshi");
        person.set_email("s19c1012");
        person.set_id(i);

        //std::cout << person.id() << std::endl;
    }
    output.close();
    std::cout << "done" << std::endl;
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
