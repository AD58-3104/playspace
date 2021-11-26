#include "infoshare.h"

using namespace Citbrains::infosharemodule;

int main(int argc, char const *argv[])
{
    std::vector<std::string> commands;
    std::vector<std::string> behaviors;
    std::string s;
    std::cout << "hello test\n";
    try
    {
        std::ifstream ifs("command_dict.dat");
        while (std::getline(ifs, s))
            commands.push_back(s);
        ifs.close();
        if (commands.empty())
        {
            throw std::string("----Error!!!---- There is nothing written in command_dict.dat");
        }
        ifs.open("behavior_name_dict.dat");
        while (std::getline(ifs, s))
            behaviors.push_back(s);
        ifs.close();
        if (commands.empty())
        {
            throw std::string("----Error!!!---- There is nothing written in behavior_name_dict.dat");
        }
    }
    catch (std::system_error &e)
    {
        std::cerr << e.what();
    }
    catch(std::string& es){
        std::cerr << es << std::endl;
        std::terminate();
    }
    try
    {
        std::cout << "checking dictionary..." << std::endl;
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
            if (itr == dict.numSequenceToCommand(dict.commandToNumSequence(itr)))
            {
                std::cout << "serialize of ->" << itr << " is succeed";
            }
            else
            {
                std::cout << "\033[31m serialize of ->" << itr << " is failed---------------------------- \033[m";
                std::cout << "result is  " << dict.numSequenceToCommand(dict.commandToNumSequence(itr)) << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------behaviors--------------------------------------------\n";
        for (const auto &itr : behaviors)
        {
            if (itr == dict.numSequenceToBehaviorName(dict.behaviorNameToNumSequence(itr)))
            {
                std::cout << "serialize of ->" << itr << " is succeed";
            }
            else
            {
                std::cout << "\033[31m serialize of ->" << itr << " is failed---------------------------- \033[m";
            }
            std::cout << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
    return 0;
}
