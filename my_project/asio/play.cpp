#include <boost/system/error_code.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    auto ec = boost::system::error_code();
    if(ec){
        std::cout << "error" << std::endl;
    }
    else{
        std::cout << "success" << std::endl;
    }
    return 0;
}
