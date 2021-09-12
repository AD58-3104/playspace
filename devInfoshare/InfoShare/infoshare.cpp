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
            : self_id_(1), our_color_(COLOR_MAGENTA), ip_adress_("127.0.0.1"), timeFunc_(nullptr), terminated_(false)
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
            //TODO implement 全部にセットする
            for(auto&& itr : robot_data_list_){
                std::lock_guard lock(itr->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::RECV_TIME)]);
                itr->timeFunc_ = func;
            }
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
        int32_t InfoShare::getOurcolor()  const noexcept
        {
            return our_color_;
        }
        int32_t InfoShare::getID()  const noexcept
        {
            return self_id_;
        }
        float InfoShare::getTime() const
        {
            if (timeFunc_ != nullptr)
            {
                return timeFunc_();
            }
            return (float)time(0);
        }

        [[nodiscard]] int32_t InfoShare::getcf_own(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->cf_own_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getcf_ball(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->cf_ball_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getstatus(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->status_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getvoltage(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->voltage_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getfps(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->fps_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::gettemperature(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->temperature_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::gethighest_servo(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->highest_servo_.load();
            }
        }
        [[nodiscard]] bool InfoShare::getis_detect_ball(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return false;
            }
            else
            {
                return robot_data_list_[id - 1]->is_detect_ball_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getstrategy_no(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->strategy_no_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getcommand(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->command_.load();
            }
        }
        [[nodiscard]] int32_t InfoShare::getcurrent_behavior_name(const int32_t &id) const noexcept
        {
            if (id == self_id_)
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->current_behavior_name_.load();
            }
        }
        [[nodiscard]] float InfoShare::getrecv_time(const int32_t &id) const
        {
            if (id == self_id_)
            {
                return 0.0;
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id-1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::RECV_TIME)]);
                return (robot_data_list_[id - 1]->recv_time_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getour_robot_gl(const int32_t &id) const
        {
            if (id == self_id_)
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id-1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::OUR_ROBOT_GL)]);
                return (robot_data_list_[id - 1]->our_robot_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getenemy_robot_gl(const int32_t &id) const
        {
            if (id == self_id_)
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id-1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::ENEMY_ROBOT_GL)]);
                return (robot_data_list_[id - 1]->enemy_robot_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getblack_pole_gl(const int32_t &id) const
        {
            if (id == self_id_)
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id-1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BLACK_POLE_GL)]);
                return (robot_data_list_[id - 1]->black_pole_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::gettarget_pos_vec(const int32_t &id) const
        {
            if (id == self_id_)
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id-1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::TARGET_POS_VEC)]);
                return (robot_data_list_[id - 1]->target_pos_vec_);
            }
        }



    }
}