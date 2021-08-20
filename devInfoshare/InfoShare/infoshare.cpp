#include "infoshare.h"
#include <array>
#include <boost/asio.hpp>
using namespace std::literals::chrono_literals;
namespace Citbrains
{
    namespace infosharemodule
    {

        InfoShare::InfoShare(const int32_t self_id, const int32_t our_color, const int32_t number_of_our_robots, const std::string ip_address, float (*timeFunc)())
            : self_id_(self_id), terminated_(false), our_color_(our_color), number_of_our_robots_(number_of_our_robots)
        {
            for (int32_t i = 0; i < number_of_our_robots_; ++i)
            {
                robot_data_list_.push_back(std::make_unique<Citbrains::infosharemodule::OtherRobotInfomation>());
            }
        }
        InfoShare::~InfoShare()
        {
            terminate();
        }

        void InfoShare::receiveSharedInfomation()
        {
            boost::asio::io_service io_service; //毎回構築していいのかしら。https://docs.microsoft.com/ja-jp/dotnet/framework/network-programming/asynchronous-server-socket-example を見ると流石に毎回構築してないので一度だけ作る。
            boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), COMM_INFO_PORT0 + self_id_));
            std::array<std::byte, 1024> receive_buffer;
            while (!terminated_)
            {
                try
                {
                    while (!terminated_)
                    {
                        if (!socket.available())
                        {
                            std::this_thread::sleep_for(50ms);
                            continue;
                        }
                        boost::system::error_code error;
                        boost::asio::ip::udp::endpoint remote_endpoint;
                        size_t receive_length = socket.receive_from(boost::asio::buffer(receive_buffer), remote_endpoint, 0, error);
                        if (error)
                            continue;
                        //いらないっぽいif(){} //TODO:ここでハッシュのチェックでもする？？ダメだったら落とすみたいな
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }

        void InfoShare::terminate()
        {
            terminated_ = true;
        }
    }
}