#include "infoshare.h"
#include <array>
#include <boost/asio.hpp>
#include <ctime>
using namespace std::literals::chrono_literals;

#ifdef INFOSHARE_DEBUG
auto debugPrint = [](CitbrainsMessage::SharingData data)
{
    try
    {
        std::cerr << "debug print " << std::endl;
        std::string s = data.cf_ball();
        if (data.has_cf_ball())
            std::cout << "string" << s << " size is " << data.cf_ball().size() << std::endl;
        int i = data.mutable_cf_ball()->at(0);
        std::cout << "int " << i << std::endl;
        if (data.has_is_detect_ball())
            std::cout << "detect flag" << (data.is_detect_ball()) << std::endl;
        if (data.has_cf_ball())
            std::cout << 1 << (static_cast<int32_t>(data.cf_ball().at(0))) << std::endl;
        if (data.has_cf_own())
            std::cout << 2 << (static_cast<int32_t>(data.cf_own().at(0))) << std::endl;
        if (data.has_status())
            std::cout << 3 << (static_cast<int32_t>(data.status().at(0))) << std::endl;
        if (data.has_fps())
            std::cout << 4 << (static_cast<int32_t>(data.fps().at(0))) << std::endl;
        if (data.has_voltage())
            std::cout << 5 << (static_cast<int32_t>(data.voltage().at(0))) << std::endl;
        if (data.has_temperature())
            std::cout << 6 << (static_cast<int32_t>(data.temperature().at(0))) << std::endl;
        if (data.has_highest_servo())
            std::cout << 7 << (static_cast<int32_t>(data.highest_servo().at(0))) << std::endl;
        if (data.has_command())
            std::cout << 8 << (static_cast<int32_t>(data.command().at(0))) << std::endl;
        if (data.has_current_behavior_name())
            std::cout << 9 << (static_cast<int32_t>(data.current_behavior_name().at(0))) << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "exception catched in" << __FILE__ << __LINE__ << std::endl;
        std::cout << e.what() << std::endl;
    }
};
#endif // INFOSHARE_DEBUG

namespace Citbrains
{
    namespace infosharemodule
    {
        //必ずsetupを呼ぶ。
        InfoShare::InfoShare()
            : self_id_(1), our_color_(COLOR_MAGENTA), timeFunc_(nullptr), terminated_(false)
        {
        }
        InfoShare::~InfoShare()
        {
            terminate();
        }

        void InfoShare::terminate()
        {
            terminated_ = true;
            server_->terminate();
            client_->terminate();
        }
        void InfoShare::changeColor(const int32_t &color)
        {
            our_color_ = color;
        }
        // void InfoShare::setTimeFunc(float (*func)())
        // {
        //     timeFunc_ = func;
        //     //TODO implement 全部にセットする
        //     for (auto &&itr : robot_data_list_)
        //     {
        //         std::lock_guard lock(itr->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::RECV_TIME)]);
        //         itr->timeFunc_ = func;
        //     }
        // }
        void InfoShare::setup(const Udpsocket::SocketMode::udpsocketmode_t mode_select, const int32_t self_id, const int32_t our_color, const std::string ip_address, float (*timefunc)())
        {
            assert((NUM_PLAYERS >= self_id) || (self_id >= 1)); // self id must be 1 or more and NUMPLAYERS or low
            static bool already_setup = false;
            if (already_setup)
            {
                std::cerr << "already setup" << std::endl;
                return;
            }
            already_setup = true;
            try
            {
                assert((0 <= self_id) && (self_id < 4));
                self_id_ = self_id;
                int32_t port = COMM_INFO_PORT0 + self_id - 1;
                assert((our_color == COLOR_MAGENTA) || (our_color == COLOR_CYAN)); // color must be magenta or cyan
                our_color_ = our_color;
                if (timefunc != nullptr)
                {
                    timeFunc_ = timefunc;
                }
                for (int32_t i = 0; i < NUM_PLAYERS; ++i)
                {
                    robot_data_list_.push_back(std::make_unique<Citbrains::infosharemodule::OtherRobotInfomation>(i, timeFunc_));
                }
                receivedDataHandler_ = [&](std::string &&data) { //全てキャプチャするのは嫌だが取り敢えずこうしておく。参照の寿命とラムダの寿命は同じ。
                                                                 //メンバにする方が良いかな。多分そっちの方が良いな
#ifdef INFOSHARE_DEBUG
                    try
                    {
#endif // INFOSHARE_DEBUG
                        std::string s(std::move(data));
                        CitbrainsMessage::SharingData shared_data;
                        shared_data.ParseFromString(s);
                        if (static_cast<int32_t>(shared_data.team_color().at(0)) != (our_color_ + CitbrainsMessage::SharingData::COLOR_OFFSET))
                            return; //他チームの情報は無視
                        if (self_id_ == static_cast<int32_t>(shared_data.id().at(0)))
                            return; //自分の情報は無視
#ifdef INFOSHARE_DEBUG
                        static int32_t received_num = 1;
                        received_num++;
                        std::cerr << "number of received " << received_num << std::endl;
                        std::cout << shared_data.DebugString() << std::endl;
#endif                                                                                                          // INFOSHARE_DEBUG
                        auto &set_target = robot_data_list_[static_cast<uint32_t>(shared_data.id().at(0)) - 1]; // atだからout of rangeで落ちる。
                        //------------data set----------------------------------------------------------------------
                        set_target->setRecv_time();
                        if (shared_data.has_cf_ball())
                        {
                            if (shared_data.cf_ball().size() == 0)
                            {
                                set_target->cf_ball_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->cf_ball_.store(static_cast<uint32_t>(shared_data.cf_ball().at(0)));
                            }
                        }
                        if (shared_data.has_cf_own())
                        {
                            if (shared_data.cf_own().size() == 0)
                            {
                                set_target->cf_own_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->cf_own_.store(static_cast<uint32_t>(shared_data.cf_own().at(0)));
                            }
                        }
                        if (shared_data.has_status())
                        {
                            if (shared_data.status().size() == 0)
                            {
                                set_target->status_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->status_.store(static_cast<uint32_t>(shared_data.status().at(0)));
                            }
                        }
                        if (shared_data.has_fps())
                        {
                            if (shared_data.fps().size() == 0)
                            {
                                set_target->fps_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->fps_.store(static_cast<uint32_t>(shared_data.fps().at(0)));
                            }
                        }
                        if (shared_data.has_voltage())
                        {
                            if (shared_data.voltage().size() == 0)
                            {
                                set_target->voltage_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->voltage_.store(static_cast<uint32_t>(shared_data.voltage().at(0)));
                            }
                        }
                        if (shared_data.has_temperature())
                        {
                            if (shared_data.temperature().size() == 0)
                            {
                                set_target->temperature_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->temperature_.store(static_cast<uint32_t>(shared_data.temperature().at(0)));
                            }
                        }
                        if (shared_data.has_highest_servo())
                        {
                            if (shared_data.highest_servo().size() == 0)
                            {
                                set_target->highest_servo_.store(static_cast<uint32_t>(0));
                            }
                            else
                            {
                                set_target->highest_servo_.store(static_cast<uint32_t>(shared_data.highest_servo().at(0)));
                            }
                        }
                        if (shared_data.has_command())
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::COMMAND)]);
                            if (shared_data.command().size() == 0)
                            {
                                set_target->command_ = "";
                            }
                            else
                            {
                                set_target->command_ = shared_data.command();
                            }
                        }
                        if (shared_data.has_current_behavior_name())
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::CURRENT_BEHAVIOR_NAME)]);
                            if (shared_data.current_behavior_name().size() == 0)
                            {
                                set_target->command_ = "";
                            }
                            else
                            {
                                set_target->command_ = shared_data.current_behavior_name();
                            }
                        }
                        if (shared_data.has_is_detect_ball())
                            set_target->is_detect_ball_.store(shared_data.is_detect_ball());
                        //--------object data set-------------------------------------------------------------------
                        if (0 < shared_data.our_robot_gl_size()) //持ってる時
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::OUR_ROBOT_GL)]);
                            set_target->our_robot_gl_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                            for (int32_t i = 0; i < shared_data.our_robot_gl_size(); i++)
                            {
                                auto itr = shared_data.mutable_our_robot_gl(i);
                                set_target->our_robot_gl_.emplace_back(static_cast<float>(itr->pos_x() / 100.0), static_cast<float>(itr->pos_y() / 100.0), static_cast<float>(itr->pos_theta() / 100.0));
                            }
                        }
                        if (0 < shared_data.enemy_robot_gl_size()) //持ってる時
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::ENEMY_ROBOT_GL)]);
                            set_target->enemy_robot_gl_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                            for (int32_t i = 0; i < shared_data.enemy_robot_gl_size(); i++)
                            {
                                auto itr = shared_data.mutable_enemy_robot_gl(i);
                                set_target->enemy_robot_gl_.emplace_back(static_cast<float>(itr->pos_x() / 100.0), static_cast<float>(itr->pos_y() / 100.0), static_cast<float>(itr->pos_theta() / 100.0));
                            }
                        }
                        if (0 < shared_data.black_pole_gl_size()) //持ってる時
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BLACK_POLE_GL)]);
                            set_target->black_pole_gl_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                            for (int32_t i = 0; i < shared_data.black_pole_gl_size(); i++)
                            {
                                auto itr = shared_data.mutable_black_pole_gl(i);
                                set_target->black_pole_gl_.emplace_back(static_cast<float>(itr->pos_x() / 100.0), static_cast<float>(itr->pos_y() / 100.0), static_cast<float>(itr->pos_theta() / 100.0));
                            }
                        }
                        if (0 < shared_data.target_pos_vec_size()) //持ってる時
                        {
                            std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::TARGET_POS_VEC)]);
                            set_target->target_pos_vec_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                            for (int32_t i = 0; i < shared_data.target_pos_vec_size(); i++)
                            {
                                auto itr = shared_data.mutable_target_pos_vec(i);
                                set_target->target_pos_vec_.emplace_back(static_cast<float>(itr->pos_x() / 100.0), static_cast<float>(itr->pos_y() / 100.0), static_cast<float>(itr->pos_theta() / 100.0));
                            }
                        }
#ifdef INFOSHARE_DEBUG
                    }
                    catch (std::system_error &e)
                    {
                        std::cerr << "error thrown in " << __FILE__ << "::" << __LINE__ << std::endl;
                        std::cerr << e.what() << std::endl;
                    }
                    std::cout << "receive end" << std::endl;
#endif // INFOSHARE_DEBUG
                };
                // if (Citbrains::Udpsocket::SocketMode::broadcast_mode == mode_select)
                // {
                //     client_ = std::make_unique<UDPClient>(ip_address, BROADCAST_PORT, mode_select); // TODO そういやブロードキャストでは？
                //     server_ = std::make_unique<UDPServer>(BROADCAST_PORT, receivedDataHandler_, mode_select, 1, ip_address);
                // }
                // else
                {
                    client_ = std::make_unique<UDPClient>("127.0.255.255", port, mode_select); // TODO そういやブロードキャストでは？
                    server_ = std::make_unique<UDPServer>(port, receivedDataHandler_, mode_select, 1, ip_address);
                }
            }
            catch (std::system_error &e)
            {
                std::cerr << "error thrown in " << __FILE__ << "::" << __LINE__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }

        int32_t InfoShare::sendCommonInfo(const Pos2DCf &ball_gl_cf, const Pos2DCf &self_pos_cf, const std::vector<Pos2D> &our_robot_gl, const std::vector<Pos2D> &enemy_robot_gl, const std::vector<Pos2D> &black_pole_gl, const int fps, const std::string &message, const std::string &behavior_name, const std::vector<Pos2D> &target_pos_vec, RobotStatus state)
        {
            CitbrainsMessage::SharingData sharing_data;
            static auto Pos2Dsetter = [](const Pos2D &input, CitbrainsMessage::Pos2D &target) -> void
            {
                target.set_pos_x(input.x);
                target.set_pos_y(input.y);
                target.set_pos_theta(input.th);
            };

            static auto Pos2DCfsetter = [](const Pos2DCf &input, CitbrainsMessage::Pos2DCf &target) -> void
            {
                char c = std::clamp(static_cast<int8_t>(input.cf), static_cast<int8_t>(0), static_cast<int8_t>(100));
                target.set_confidence(std::string{c}.c_str());
                target.set_is_detect(input.is_detect);
                target.mutable_position()->set_pos_x(input.pos.x);
                target.mutable_position()->set_pos_y(input.pos.y);
                target.mutable_position()->set_pos_theta(input.pos.th);
            };

            Pos2DCfsetter(ball_gl_cf, *(sharing_data.mutable_ball_gl_cf()));
            Pos2DCfsetter(ball_gl_cf, *(sharing_data.mutable_self_pos_cf()));
            for (const auto &input : our_robot_gl)
            {
                auto target_pos2d = sharing_data.add_our_robot_gl();
                Pos2Dsetter(input, *target_pos2d);
            }
            for (const auto &input : enemy_robot_gl)
            {
                auto target_pos2d = sharing_data.add_enemy_robot_gl();
                Pos2Dsetter(input, *target_pos2d);
            }
            for (const auto &input : black_pole_gl)
            {
                auto target_pos2d = sharing_data.add_black_pole_gl();
                Pos2Dsetter(input, *target_pos2d);
            }
            for (const auto &input : target_pos_vec)
            {
                auto target_pos2d = sharing_data.add_target_pos_vec();
                Pos2Dsetter(input, *target_pos2d);
            }
            char status = (state.Posture != STATE_POSTURE_STAND) ? 0x01 : 0x00; // 転倒のフラグ TODO 読みやすくする
            if (state.Active == STATE_IDLE)
                status |= 0x02;                                   // アイドル状態かどうかのフラグ
            char voltage = 0xff & (state.MotorVoltage >> 3);      // モータの電圧(mV) >> 3
            char temperature = 0xff & state.Temperature;          // モータの温度(degree)
            char highest_servo = 0xff & (state.Temperature >> 8); // 最も温度の高いモータ
            sharing_data.set_command(message);
            sharing_data.set_current_behavior_name(behavior_name);
            sharing_data.set_id(std::string{static_cast<char>(self_id_)});
            sharing_data.set_team_color(std::string{static_cast<char>(CitbrainsMessage::SharingData::COLOR_OFFSET + our_color_)});
            sharing_data.set_status(std::string{static_cast<char>(status)});
            sharing_data.set_voltage(std::string{static_cast<char>(voltage)});
            sharing_data.set_temperature(std::string{static_cast<char>(temperature)});
            sharing_data.set_highest_servo(std::string{static_cast<char>(highest_servo)});
            std::string s = sharing_data.SerializeAsString();
            client_->send(std::move(s));
            return 0;
        }
        std::string InfoShare::getBroadcastIP(const std::string& ip_address)
        {
            // ブロードキャスト用にIPアドレスを変更
            std::string broadcastip ip_address;
            try
            {
                broadcastip = ip_address;
                broadcastip.erase(broadcastip.find_last_of("."));
                broadcastip += ".255";
            }
            catch (std::exception)
            {
                broadcastip = "255.255.255.255";
            }

            return broadcastip;
        }

        //----------------------------simple getter--------------------------------------------------------
        int32_t InfoShare::getOurcolor() const noexcept
        {
            return our_color_;
        }
        int32_t InfoShare::getID() const noexcept
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
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
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return 0;
            }
            else
            {
                return robot_data_list_[id - 1]->strategy_no_.load();
            }
        }
        [[nodiscard]] std::string InfoShare::getcommand(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return "";
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::COMMAND)]);
                return (robot_data_list_[id - 1]->command_);
            }
        }
        [[nodiscard]] std::string InfoShare::getcurrent_behavior_name(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return "";
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::CURRENT_BEHAVIOR_NAME)]);
                return (robot_data_list_[id - 1]->current_behavior_name_);
            }
        }
        [[nodiscard]] float InfoShare::getrecv_time(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return 0.0;
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::RECV_TIME)]);
                return (robot_data_list_[id - 1]->recv_time_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getour_robot_gl(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::OUR_ROBOT_GL)]);
                return (robot_data_list_[id - 1]->our_robot_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getenemy_robot_gl(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::ENEMY_ROBOT_GL)]);
                return (robot_data_list_[id - 1]->enemy_robot_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::getblack_pole_gl(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BLACK_POLE_GL)]);
                return (robot_data_list_[id - 1]->black_pole_gl_);
            }
        }
        [[nodiscard]] std::vector<Pos2D> InfoShare::gettarget_pos_vec(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2D()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::TARGET_POS_VEC)]);
                return (robot_data_list_[id - 1]->target_pos_vec_);
            }
        }
        [[nodiscard]] Pos2DCf InfoShare::getball_gl_cf(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2DCf()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BALL_GL_CF)]);
                return (robot_data_list_[id - 1]->ball_gl_cf_);
            }
        }
        [[nodiscard]] Pos2DCf InfoShare::getself_pos_cf(const int32_t &id) const
        {
            if ((id == self_id_) || (id < 1) || (NUM_PLAYERS < id))
            {
                return {Pos2DCf()};
            }
            else
            {
                std::lock_guard lock(robot_data_list_[id - 1]->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::SELF_POS_CF)]);
                return (robot_data_list_[id - 1]->self_pos_cf_);
            }
        }
    }
}
