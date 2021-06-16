#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <cmath>
#include <algorithm>
using namespace std;

class file_input
{
    //This class is written by 19C1012 Inoue Satoshi.
public:
    using data_t = std::vector<std::vector<std::vector<std::vector<float>>>>;
    file_input();
    const std::string root = "/home/satoshi/playspace/kadai/ninshiki/city_mcepdata/";
    std::vector<std::string> cities = {"city011", "city012", "city021", "city022"};
    std::vector<std::vector<std::string>> filenames;
    int dpMatching(int tmpfolder, int tmpfile_num, int folder);
    data_t getfiledatas();

private:
    float local_distance(const std::vector<float> &frame_i, const std::vector<float> &frame_j);
    static const int dimension = 15;
    std::ifstream ifs;
    data_t data; //[フォルダ][ファイルが何番目か][行][列]
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
file_input::data_t file_input::getfiledatas()
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
            ifs >> s;
            int rows = 0;
            ifs >> rows;
            data[city_num][file_count].resize(rows);
            for (int i = 0; i < rows; ++i)
            {
                float tmp;
                for (int j = 0; j < dimension; ++j)
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
    return this->data;
}

int file_input::dpMatching(int tmpfolder, int tmpfile_num, int target_folder)
{
    float min_distance = 1e9;
    auto template_data = data[tmpfolder][tmpfile_num];
    int voice_num = 0;
    int shortest = 0;
    for (const auto &target : data[target_folder])
    {
        int max_i = template_data.size();
        int max_j = target.size();
        std::vector<std::vector<double>> result(template_data.size(), std::vector<double>(target.size(), 1e8));
        result[0][0] = local_distance(template_data[0], target[0]);

        for (int tmp_i = 0; tmp_i < max_i; ++tmp_i)
        {
            for (int target_j = 0; target_j < max_j; ++target_j)
            {
                if (tmp_i == 0 && target_j == 0)
                    continue;
                double min_num = 1e10;
                if (tmp_i - 1 >= 0)
                    min_num = std::min(min_num, local_distance(template_data[tmp_i], target[target_j]) + result[tmp_i - 1][target_j]);
                if (target_j - 1 >= 0)
                    min_num = std::min(min_num, local_distance(template_data[tmp_i], target[target_j]) + result[tmp_i][target_j - 1]);
                if ((tmp_i - 1 >= 0) && (target_j - 1 >= 0))
                    min_num = std::min(min_num, 10 * local_distance(template_data[tmp_i], target[target_j]) + result[tmp_i - 1][target_j - 1]);
                result[tmp_i][target_j] = min_num;
            }
        }
        if (min_distance >= result[max_i - 1][max_j - 1] / (max_i + max_j))
        {
            min_distance = result[max_i - 1][max_j - 1] / (max_i + max_j);
            shortest = voice_num;
        }
        voice_num++;
    }
    if (shortest == tmpfile_num)
        return 1;
    else
        return 0;
}

float file_input::local_distance(const std::vector<float> &frame_i, const std::vector<float> &frame_j)
{
    float result_distance = 0;
    for (int i = 0; i < dimension; ++i)
    {
        result_distance += (frame_i[i] - frame_j[i]) * (frame_i[i] - frame_j[i]);
    }
    return std::sqrt(result_distance);
}

int main(int argc, char const *argv[])
{
    file_input files;
    files.getfiledatas();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int n = 0;
            if (i == j)
            {
                continue;
            }
            for (int k = 0; k < 100; ++k)
            {
                n += files.dpMatching(i, k, j);
            }
            std::cout << "template::" << files.cities[i] << " compared::" << files.cities[j] << std::endl;
            std::cout << "result:: " << n << "%" << std::endl;
        }
    }

    return 0;
}
