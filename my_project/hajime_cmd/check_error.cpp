#include <zmq.hpp>
#include <iostream>
#include <string>
void f(){
    throw zmq::error_t();
}

int main(int argc, char const *argv[])
{
    try{
        f();
    }
    catch(std::exception e){
        std::string s(e.what());
        std::cout << s << std::endl;
    }
    zmq::send_result_t a;
    std::cout << "end" << std::endl; 
    return 0;
}
