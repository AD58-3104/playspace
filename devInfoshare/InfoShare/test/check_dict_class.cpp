#include "infoshare.h"

int main(int argc, char const *argv[])
{
    std::vector<std::string> commands;
    std::vector<std::string> behaviors;
    std::string s;
    std::ifstream ifs("command_dict.dat");
    while (std::getline(ifs, s))
        commands.push_back(s);
    ifs.close();
    ifs.open("behavior_name_dict.dat");
    while (std::getline(ifs, s))
        behaviors.push_back(s);
    ifs.close();
    try
    {
        int32_t cmd_len = commands.size();
        int32_t behv_len = behaviors.size();
        for (int i = 0; i < cmd_len; ++i)
        {
            commands.push_back(commands[i] + commands[cmd_len - 1 - i] + commands[commands.size() - 1]); //最後の方2倍に近い形で長くなるので注意
        }
        for (int i = 0; i < behv_len; ++i)
        {
            behaviors.push_back(behaviors[i] + behaviors[behv_len - 1 - i] + behaviors[behaviors.size() - 1]);
        }
        commands.push_back(commands[2] + commands[1]);
        Citbrains::infosharemodule::SerializeStringByDict dict;
        for (const auto &itr : commands)
        {
            std::cout << itr << " is ";
            for (const int32_t &num : dict.commandToNumSequence(itr))
                std::cout << num;
            std::cout << std::endl;
        }
        for (const auto &itr : behaviors)
        {
            std::cout << itr << " is ";
            for (const int32_t &num : dict.behaviorNameToNumSequence(itr))
                std::cout << num;
            std::cout << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
    return 0;
}
