#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <cstdint>
#include <list>
#include <algorithm>
using namespace std;

class file_input
{
public:
    file_input();
    std::ifstream ifs;
    std::vector<std::string> cities = {"city011", "city012", "city021", "city022"};
    std::string root = "/home/satoshi/playspace/kadai/ninshiki/city_mcepdata/";
    std::vector<std::vector<std::string>> filenames;
    std::vector<std::vector<std::vector<std::vector<float>>>> data; //[フォルダ][ファイルが何番目か][行][列]

    void getfiledatas();
};

file_input::file_input()
{
    filenames.resize(4);
    data.resize(4);
    for (auto &a : data)
    {
        a.resize(100);
    }
}
void file_input::getfiledatas()
{
    int city_num = 0;
    for (const auto &city : cities)
    {
        std::string now = root + city;
        for (const std::filesystem::directory_entry &i : std::filesystem::directory_iterator(now))
        {
            filenames[city_num].push_back(i.path().filename().string());
        }
        std::sort(filenames[city_num].begin(), filenames[city_num].end());
        int file_count = 0;
        for (const auto &name : filenames[city_num])
        {
            ifs.open(now + "/" + name);
            std::string s;
            ifs >> s;
            std::cout << s << std::endl;
            ifs >> s;
            int rows = 0;
            ifs >> rows;
            data[city_num][file_count].resize(rows);
            for (int i = 0; i < rows; ++i)
            {
                float tmp;
                for (int j = 0; j < 15; ++j)
                {
                    ifs >> tmp;
                    data[city_num][file_count][i].push_back(tmp);
                }
            }
            ifs.close();
            file_count++;
        }
        city_num++;
    }
}

int main(int argc, char const *argv[])
{
    file_input files;
    files.getfiledatas();
    for (auto &a : files.data[0][0])
    {
        for (auto &b : a)
        {
            std::cout << b << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
