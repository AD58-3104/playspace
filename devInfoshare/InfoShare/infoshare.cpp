#include "infoshare.h"
#include <array>
#include <boost/asio.hpp>
#include <ctime>
using namespace std::literals::chrono_literals;
namespace Citbrains
{
    namespace infosharemodule
    {
        //デフォルトの時間関数を使いたい場合は引数無し
        //必ずsetupを呼ぶ。
        InfoShare::InfoShare()
            : self_id_(1), our_color_(COLOR_MAGENTA), ip_adress_("127.0.0.1"), timeFunc_(timefunc), terminated_(false)
        {
        }
        InfoShare::~InfoShare()
        {
            terminate();
        }

        // void InfoShare::receiveSharedInfomation()
        // {
        //     boost::asio::io_service io_service; //毎回構築していいのかしら。https://docs.microsoft.com/ja-jp/dotnet/framework/network-programming/asynchronous-server-socket-example を見ると流石に毎回構築してないので一度だけ作る。
        //     boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), COMM_INFO_PORT0 + self_id_));
        //     std::array<std::byte, 1024> receive_buffer;
        //     while (!terminated_)
        //     {
        //         try
        //         {
        //             while (!terminated_)
        //             {
        //                 if (!socket.available())
        //                 {
        //                     std::this_thread::sleep_for(50ms);
        //                     continue;
        //                 }
        //                 boost::system::error_code error;
        //                 boost::asio::ip::udp::endpoint remote_endpoint;
        //                 size_t receive_length = socket.receive_from(boost::asio::buffer(receive_buffer), remote_endpoint, 0, error);
        //                 if (error)
        //                     continue;
        //                 //いらないっぽいif(){} //TODO:ここでハッシュのチェックでもする？？ダメだったら落とすみたいな
        //             }
        //         }
        //         catch (const std::exception &e)
        //         {
        //             std::cerr << e.what() << std::endl;
        //         }
        //     }
        // }
        void InfoShare::terminate()
        {
            //clientとserverのio_serviceをstopする為のterminateを呼ぶ。
            terminated_ = true;
        }
        void InfoShare::changeColor(const int32_t color)
        {
            our_color_ = color;
        }
        void InfoShare::setTimeFunc(float (*func)())
        {
            timeFunc_ = func;
        }
        //既にtimefuncを設定済みの場合は渡さなければ変更されない。
        void InfoShare::setup(const int32_t self_id = 1, const int32_t our_color = COLOR_MAGENTA, const std::string ip_adress = "127.0.0.1", float (*timefunc)() = nullptr)
        {
            self_id_ = self_id;
            ip_adress_ = ip_adress;
            our_color_ = our_color;
            if (timefunc != nullptr)
            {
                timeFunc_ = timefunc;
            }
            for (int32_t i = 0; i < NUM_PLAYERS; ++i)
            {
                robot_data_list_.push_back(std::make_unique<Citbrains::infosharemodule::OtherRobotInfomation>(i + 1, timeFunc_));
            }
            receivedDataHandler_ = [&](std::string &&data) { //全てキャプチャするのは嫌だが取り敢えずこうしておく。参照の寿命とラムダの寿命は同じ。
                //パースして中に入れる。大分面倒なので後で.....。
                std::string s(std::move(data));
                CitbrainsMessage::SharingData shared_data;
                shared_data.ParseFromString(s);
                //ここで必ずsetRecv_timeを呼びます。
            };
        }
        float InfoShare::getTime()
        {
            if (timeFunc_ != nullptr)
            {
                return timeFunc_();
            }
            return (float)time(0);
        }

        int32_t InfoShare::getOurcolor()
        {
            return our_color_;
        }
    }
}