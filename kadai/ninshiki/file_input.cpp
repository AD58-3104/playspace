#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <cstdint>
using namespace std;

class file_input
{
    std::vector<std::string> cities = {"city011","city012","city021","city022"};
    std::string root = "~/playspace/kadai/ninhiki/city_mcepdata/";
    std::vector<std::vector<std::string>> filenames;
};


void getfilenames(){
    for(const auto &city :cities){
    std::string now = root + city;
    for(const auto &:)
    }
}


int main(int argc, char const *argv[])
{

    for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator("."))
    {
        std::cout << i.path().filename().string() << std::endl;
    }
    return 0;
}
