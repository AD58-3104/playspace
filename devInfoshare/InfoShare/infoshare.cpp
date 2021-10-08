#include "infoshare.h"
#include <array>
#include <boost/asio.hpp>
#include <ctime>
#include <type_traits>
#include <google/protobuf/util/time_util.h>
#include <fstream>
using namespace std::literals::chrono_literals;

namespace debug
{
    static std::vector<std::pair<int32_t,int64_t>> time_log;
    /**
     * @brief protobufオブジェクトのデバッグ関数.INFOSHARE_DEBUGがONの時のみ有効.debugの時に適当に書き換えていいです.
     * @param[in] data protobufのオブジェクト
     */
    static void debugPrint(const CitbrainsMessage::SharingData &data)
    {
        static int32_t received_num = 1;
        received_num++;
        // std::cerr << "number of received " << received_num << std::endl;
        // std::cout << data.DebugString() << "\n"; //正確に時間計測するならこの二行の出力は無くすべき.
        if (data.has_timestamp())
        {
            using namespace google::protobuf::util;
            time_log.push_back(std::make_pair(received_num,TimeUtil::DurationToNanoseconds(TimeUtil::NanosecondsToTimestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) - data.timestamp())));
        }
        try
        {
            // std::cerr << "debug print " << std::endl;
            // std::string s = data.cf_ball();
            // if (data.has_cf_ball())
            //     std::cout << "string" << s << " size is " << data.cf_ball().size() << std::endl;
            // int i = data.cf_ball().at(0);
            // std::cout << "int " << i << std::endl;
            // if (data.has_is_detect_ball())
            //     std::cout << "detect flag" << (data.is_detect_ball()) << std::endl;
            // if (data.has_cf_ball())
            //     std::cout << 1 << (static_cast<int32_t>(data.cf_ball().at(0))) << std::endl;
            // if (data.has_cf_own())
            //     std::cout << 2 << (static_cast<int32_t>(data.cf_own().at(0))) << std::endl;
            // if (data.has_status())
            //     std::cout << 3 << (static_cast<int32_t>(data.status().at(0))) << std::endl;
            // if (data.has_fps())
            //     std::cout << 4 << (static_cast<int32_t>(data.fps().at(0))) << std::endl;
            // if (data.has_voltage())
            //     std::cout << 5 << (static_cast<int32_t>(data.voltage().at(0))) << std::endl;
            // if (data.has_temperature())
            //     std::cout << 6 << (static_cast<int32_t>(data.temperature().at(0))) << std::endl;
            // if (data.has_highest_servo())
            //     std::cout << 7 << (static_cast<int32_t>(data.highest_servo().at(0))) << std::endl;
            // if (data.has_command())
            //     std::cout << 8 << (static_cast<int32_t>(data.command().at(0))) << std::endl;
            // if (data.has_current_behavior_name())
            //     std::cout << 9 << (static_cast<int32_t>(data.current_behavior_name().at(0))) << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "exception catched in" << __FILE__ << __LINE__ << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
    /**
     * @brief protobufオブジェクトのデバッグを終了する時に呼ぶ関数.主にファイルに書き込む.
     */
    void finishDebug()
    {
        std::ofstream ofs;
        ofs.open("receive_time_log.txt");
        for(const auto& [receive_num,time_nanos]:time_log){
            ofs << "number " << receive_num << " reached " << time_nanos << "nano seconds" << std::endl;
        }
        ofs.close();
    }

}
namespace Citbrains
{
    namespace infosharemodule
    {
        static inline constexpr float INT_FLOAT_CONVERT_SCALE = 1000.0; // floatで送ると効率が良くないのでintに変換して送信している.今は小数点以下3桁.
        /**
         * @brief コンストラクタ
         * @details コンストラクタは何もしないので必ずsetupを呼び初期化する.
         */
        InfoShare::InfoShare()
            : self_id_(1), our_color_(COLOR_MAGENTA), timeFunc_(nullptr), terminated_(false), dictionary()
        {
        }
        /**
         * @brief デストラクタ
         * @details 必ずterminateを呼ぶ.
         */
        InfoShare::~InfoShare()
        {
            terminate();
#ifdef INFOSHARE_DEBUG
            debug::finishDebug();
#endif // INFOSHARE_DEBUG
        }

        /**
         * @brief 終了関数
         * @details 終了したい時に呼ぶ.デストラクタでも呼ばれる.
         */
        void InfoShare::terminate()
        {
            terminated_ = true;
            server_->terminate();
            client_->terminate();
        }

        /**
         * @brief 色の変更をする
         * @param[in] color COLOR_MAGENTA(4) or COLOR_CYAN(5) in hpl_types.h
         * @details COLOR_CYAN or COLOR_MAGENTA以外が入力された場合は何も変更しない.
         */
        void InfoShare::changeColor(const int32_t &color) noexcept
        {
            if ((color != COLOR_CYAN) && (color != COLOR_MAGENTA))
                return;
            our_color_ = color;
        }

        /**
         * @brief 初期化と通信開始の関数.呼び出しは一度しか許容されない.
         * @param[in] mode_select 通信モード選択(基本はbroadcastにする)
         * @param[in] self_id ロボットのID 1 ～ NUM_PLAYERS
         * @param[in] our_color ロボットの色 COLOR_MAGENTA(4), COLOR_CYAN(5)
         * @param[in] ip_address IPアドレス.デフォルトだとbroadcast用のアドレスが入力される.
         * @param[in] timefunc 受信時間記録の為の関数ポインタ.デフォルトはtime.h(ctime)のtime().
         * @return void
         * @details UDPサーバとクライアントのインスタンス生成とinfoshareオブジェクトの初期化を行う.idは1-indexed.
         */
        void InfoShare::setup(const Udpsocket::SocketMode::udpsocketmode_t &mode_select, const int32_t &self_id, const int32_t &our_color, const std::string &ip_address, float (*timefunc)())
        {
            static bool already_setup = false;
            if (already_setup)
            {
                std::cerr << "already setup in this process" << std::endl;
                return;
            }
            already_setup = true;
            try
            {
                assert((NUM_PLAYERS >= self_id) || (self_id >= 1));                // self id must be 1 or more and NUMPLAYERS or low
                assert((our_color == COLOR_MAGENTA) || (our_color == COLOR_CYAN)); // color must be magenta or cyan
                self_id_ = self_id;
                our_color_ = our_color;

                if (timefunc != nullptr)
                {
                    timeFunc_ = timefunc;
                }

                for (int32_t i = 0; i < NUM_PLAYERS; ++i)
                {
                    robot_data_list_.push_back(std::make_unique<Citbrains::infosharemodule::OtherRobotInfomation>(i, timeFunc_));
                }

                if (mode_select == Citbrains::Udpsocket::SocketMode::broadcast_mode)
                {
                    int32_t port = BROADCAST_PORT;
                    std::string broadcastip = toBroadcastIP(ip_address);
                    client_ = std::make_unique<UDPClient>(broadcastip, port, mode_select);
                    server_ = std::make_unique<UDPServer>(
                        port, [this](std::string &&s)
                        { receivedDataHandler(std::move(s)); },
                        mode_select, 1, broadcastip);
                }
                else
                {
                    int32_t port = COMM_INFO_PORT0 + self_id - 1;
                    client_ = std::make_unique<UDPClient>(ip_address, port, mode_select);
                    server_ = std::make_unique<UDPServer>(
                        port, [this](std::string &&s)
                        { receivedDataHandler(std::move(s)); },
                        mode_select, 1, ip_address);
                }
            }
            catch (std::exception &e)
            {
                std::cerr << "error catched in " << __FILE__ << "::" << __LINE__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }
        /**
         * @brief 情報をシリアライズして送信.
         * @param[in] ball_gl_cf ボールの確信度付き位置情報.
         * @param[in] self_pos_cf 自己位置の確信度付き位置情報.
         * @param[in] our_robot_gl 自チームのロボットのグローバル座標
         * @param[in] enemy_robot_gl 敵チームロボットのグローバル座標
         * @param[in] black_pole_gl その他の障害物のグローバル座標
         * @param[in] fps HPLのメインループの周期。モニター用 8bit
         * @param[in] message roleとgoalが組み合わさった文字列
         * @param[in] behavior_name ロボットが現在行っているbehaviorの文字列
         * @param[in] target_pos_vec ロボットが向かおうとしている座標
         * @param[in] state ロボットの現在の状態(転倒しているかどうか,voltage,サーボ温度等)
         * @todo behaviorとmessageの辞書によるシリアライズをする
         * @return void
         */
        void InfoShare::sendCommonInfo(const Pos2DCf &ball_gl_cf, const Pos2DCf &self_pos_cf, const std::vector<Pos2D> &our_robot_gl, const std::vector<Pos2D> &enemy_robot_gl, const std::vector<Pos2D> &black_pole_gl, const int &fps, const std::string &message, const std::string &behavior_name, const std::vector<Pos2D> &target_pos_vec, RobotStatus state)
        {
            CitbrainsMessage::SharingData sharing_data;
            // 100倍にしてintにしてから送る.受け取った時に戻す.
            static auto Pos2Dsetter = [](const Pos2D &input, CitbrainsMessage::Pos2D &target) -> void
            {
                target.set_pos_x(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.x));
                target.set_pos_y(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.y));
                target.set_pos_theta(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.th));
            };
            // 100倍してintにしてから送る.受け取った時に戻す.
            static auto Pos2DCfsetter = [](const Pos2DCf &input, CitbrainsMessage::Pos2DCf &target) -> void
            {
                char c = std::clamp<uint8_t>(static_cast<uint8_t>(input.cf), static_cast<uint8_t>(0), static_cast<uint8_t>(100));
                target.set_confidence(std::string{c}.c_str());
                target.set_is_detect(input.is_detect);
                target.mutable_position()->set_pos_x(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.pos.x));
                target.mutable_position()->set_pos_y(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.pos.y));
                target.mutable_position()->set_pos_theta(static_cast<int32_t>(INT_FLOAT_CONVERT_SCALE * input.pos.th));
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
                status |= 0x02;                                            // アイドル状態かどうかのフラグ
            unsigned char voltage = 0xff & (state.MotorVoltage >> 3);      // モータの電圧(mV) >> 3
            unsigned char temperature = 0xff & state.Temperature;          // モータの温度(degree)
            unsigned char highest_servo = 0xff & (state.Temperature >> 8); // 最も温度の高いモータ
            sharing_data.set_id(std::string{static_cast<char>(self_id_)});
            sharing_data.set_team_color(std::string{static_cast<char>(CitbrainsMessage::SharingData::COLOR_OFFSET + our_color_)});
            sharing_data.set_status(std::string{static_cast<char>(status)});
            sharing_data.set_fps(std::string{static_cast<char>(fps)});
            sharing_data.set_voltage(std::string{static_cast<char>(voltage)});
            sharing_data.set_temperature(std::string{static_cast<char>(temperature)});
            sharing_data.set_highest_servo(std::string{static_cast<char>(highest_servo)});
            sharing_data.set_command(dictionary.commandToNumSequence(message));
            sharing_data.set_current_behavior_name(dictionary.behaviorNameToNumSequence(behavior_name));
            std::string s = sharing_data.SerializeAsString();
            client_->send(std::move(s));
        }

        /**
         * @brief broadcast用アドレスへの変換
         * @param[in] ip_address 変換元のアドレス
         * @return string broadcast用のIPアドレス
         */
        std::string InfoShare::toBroadcastIP(const std::string &ip_address)
        {
            std::string broadcastip;
            try
            {
                broadcastip = ip_address;
                broadcastip.erase(broadcastip.find_last_of("."));
                broadcastip += ".255";
            }
            catch (std::exception &e)
            {
                broadcastip = "255.255.255.255";
            }
            return broadcastip;
        }

        /**
         * @brief UDPClientが受け取った情報のハンドラ.
         * @param[in] data UDPのクライアントが受け取ったバイト列
         * @details 受け取ったバイト列をシリアライズしてOtherRobotInfomationに格納する.protobufのbytesは0の場合壊れるのでわざわざ分岐している.
         */
        void InfoShare::receivedDataHandler(std::string &&data)
        {
            try
            {

                CitbrainsMessage::SharingData shared_data;
                shared_data.ParseFromString(std::move(data));
                if (static_cast<int32_t>(shared_data.team_color().at(0)) != (our_color_ + CitbrainsMessage::SharingData::COLOR_OFFSET))
                {
#ifdef INFOSHARE_DEBUG
                    std::cout << "color is " << static_cast<int32_t>(shared_data.team_color().at(0)) << " another team color " << std::endl;
#endif                      // INFOSHARE_DEBUG
                    return; //他チームの情報は無視
                }
                if (self_id_ == static_cast<int32_t>(shared_data.id().at(0)))
                {
#ifdef INFOSHARE_DEBUG
                    std::cout << "my infomation" << std::endl;
#endif                      // INFOSHARE_DEBUG
                    return; //自分の情報は無視
                }
#ifdef INFOSHARE_DEBUG
                debug::debugPrint(shared_data);
#endif // INFOSHARE_DEBUG
                auto &set_target = robot_data_list_[static_cast<int32_t>(shared_data.id().at(0)) - 1];
                //------------data set-----------------------------------------------------------------
                set_target->setRecv_time();
                if (shared_data.has_cf_ball())
                {
                    if (shared_data.cf_ball().size() == 0)
                    {
                        set_target->cf_ball_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->cf_ball_.store(static_cast<int32_t>(shared_data.cf_ball().at(0)));
                    }
                }
                if (shared_data.has_cf_own())
                {
                    if (shared_data.cf_own().size() == 0)
                    {
                        set_target->cf_own_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->cf_own_.store(static_cast<int32_t>(shared_data.cf_own().at(0)));
                    }
                }
                if (shared_data.has_status())
                {
                    if (shared_data.status().size() == 0)
                    {
                        set_target->status_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->status_.store(static_cast<int32_t>(shared_data.status().at(0)));
                    }
                }
                if (shared_data.has_fps())
                {
                    if (shared_data.fps().size() == 0)
                    {
                        set_target->fps_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->fps_.store(static_cast<int32_t>(shared_data.fps().at(0)));
                    }
                }
                if (shared_data.has_voltage())
                {
                    if (shared_data.voltage().size() == 0)
                    {
                        set_target->voltage_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->voltage_.store(static_cast<int32_t>(shared_data.voltage().at(0)));
                    }
                }
                if (shared_data.has_temperature())
                {
                    if (shared_data.temperature().size() == 0)
                    {
                        set_target->temperature_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->temperature_.store(static_cast<int32_t>(shared_data.temperature().at(0)));
                    }
                }
                if (shared_data.has_highest_servo())
                {
                    if (shared_data.highest_servo().size() == 0)
                    {
                        set_target->highest_servo_.store(static_cast<int32_t>(0));
                    }
                    else
                    {
                        set_target->highest_servo_.store(static_cast<int32_t>(shared_data.highest_servo().at(0)));
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
                        set_target->command_ = dictionary.numSequenceToCommand(shared_data.command());
                    }
                }
                if (shared_data.has_current_behavior_name())
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::CURRENT_BEHAVIOR_NAME)]);
                    if (shared_data.current_behavior_name().size() == 0)
                    {
                        set_target->current_behavior_name_ = "";
                    }
                    else
                    {
                        set_target->current_behavior_name_ = dictionary.numSequenceToBehaviorName(shared_data.current_behavior_name());
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
                        set_target->our_robot_gl_.emplace_back(static_cast<float>(itr->pos_x() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_y() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_theta() / INT_FLOAT_CONVERT_SCALE));
                    }
                }
                if (0 < shared_data.enemy_robot_gl_size()) //持ってる時
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::ENEMY_ROBOT_GL)]);
                    set_target->enemy_robot_gl_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                    for (int32_t i = 0; i < shared_data.enemy_robot_gl_size(); i++)
                    {
                        auto itr = shared_data.mutable_enemy_robot_gl(i);
                        set_target->enemy_robot_gl_.emplace_back(static_cast<float>(itr->pos_x() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_y() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_theta() / INT_FLOAT_CONVERT_SCALE));
                    }
                }
                if (0 < shared_data.black_pole_gl_size()) //持ってる時
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BLACK_POLE_GL)]);
                    set_target->black_pole_gl_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                    for (int32_t i = 0; i < shared_data.black_pole_gl_size(); i++)
                    {
                        auto itr = shared_data.mutable_black_pole_gl(i);
                        set_target->black_pole_gl_.emplace_back(static_cast<float>(itr->pos_x() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_y() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_theta() / INT_FLOAT_CONVERT_SCALE));
                    }
                }
                if (0 < shared_data.target_pos_vec_size()) //持ってる時
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::TARGET_POS_VEC)]);
                    set_target->target_pos_vec_.clear(); //どうせまた格納されるのでshrink_to_fitしない。
                    for (int32_t i = 0; i < shared_data.target_pos_vec_size(); i++)
                    {
                        auto itr = shared_data.mutable_target_pos_vec(i);
                        set_target->target_pos_vec_.emplace_back(static_cast<float>(itr->pos_x() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_y() / INT_FLOAT_CONVERT_SCALE), static_cast<float>(itr->pos_theta() / INT_FLOAT_CONVERT_SCALE));
                    }
                }
                if (shared_data.has_ball_gl_cf())
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::BALL_GL_CF)]);
                    set_target->ball_gl_cf_.cf = shared_data.ball_gl_cf().confidence()[0];
                    set_target->ball_gl_cf_.is_detect = shared_data.ball_gl_cf().is_detect();
                    set_target->ball_gl_cf_.pos.x = static_cast<float>(shared_data.ball_gl_cf().position().pos_x() / INT_FLOAT_CONVERT_SCALE);
                    set_target->ball_gl_cf_.pos.y = static_cast<float>(shared_data.ball_gl_cf().position().pos_y() / INT_FLOAT_CONVERT_SCALE);
                    set_target->ball_gl_cf_.pos.th = static_cast<float>(shared_data.ball_gl_cf().position().pos_theta() / INT_FLOAT_CONVERT_SCALE);
                }
                if (shared_data.has_self_pos_cf())
                {
                    std::lock_guard lock(set_target->dataMutexes_[static_cast<int32_t>(OtherRobotInfomation::MutexTag::SELF_POS_CF)]);
                    set_target->self_pos_cf_.cf = shared_data.self_pos_cf().confidence()[0];
                    set_target->self_pos_cf_.is_detect = shared_data.self_pos_cf().is_detect();
                    set_target->self_pos_cf_.pos.x = static_cast<float>(shared_data.self_pos_cf().position().pos_x() / INT_FLOAT_CONVERT_SCALE);
                    set_target->self_pos_cf_.pos.y = static_cast<float>(shared_data.self_pos_cf().position().pos_y() / INT_FLOAT_CONVERT_SCALE);
                    set_target->self_pos_cf_.pos.th = static_cast<float>(shared_data.self_pos_cf().position().pos_theta() / INT_FLOAT_CONVERT_SCALE);
                }
            }
            catch (std::exception &e)
            {
                std::cerr << "error catched in " << __FILE__ << "::" << __LINE__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }

        /**
         * @brief 自分達の色の取得
         * @return int32_t
         */
        int32_t InfoShare::getOurcolor() const noexcept
        {
            return our_color_;
        }
        /**
         * @brief ロボットIDの取得
         * @return int32_t
         */
        int32_t InfoShare::getID() const noexcept
        {
            return self_id_;
        }
        /**
         * @brief 現在時間の取得.デフォルトはtime.h(ctime)のtime(0).
         * @return float
         */
        float InfoShare::getTime() const
        {
            if (timeFunc_ != nullptr)
            {
                return timeFunc_();
            }
            return (float)time(0);
        }

        //共有された情報のgetter------------------------------------------------------------------------
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return int32_t
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return string
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return string
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return float
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return std::vector<Pos2D>
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return std::vector<Pos2D>
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return std::vector<Pos2D>
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return std::vector<Pos2D>
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return Pos2DCf
         */
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
        /**
         * @brief 関数名の変数に対するgetter
         * @param[in] id 情報が欲しいロボットの番号. id == self_idの時,また(1 < id < NUM_PLAYERS)を満たさない場合は0に値するものを返す.
         * @return Pos2DCf
         */
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

        /**
         * @brief commandの文字列を送信する前に辞書を参照して最小単位毎に数字に変える
         * @param[in] command 送信されるcommandの文字列
         * @return 辞書の対応する番号で構成される数字列
         * @details pyfilesディレクトリ内の辞書を参考にして数字に変換
         */
        SerializeStringByDict::ReturnNumSequenceType SerializeStringByDict::commandToNumSequence(const std::string &command)
        {
            std::string word;
            SerializeStringByDict::ReturnNumSequenceType return_seq;
            auto not_contain = command_to_num.end();
            try
            {
                for (const auto &character : command)
                {
                    word.push_back(character);
                    if (command_to_num.find(word) != not_contain)
                    {
                        return_seq.push_back(command_to_num[word]);
                        word.clear();
                    }
                }
            }
            catch (std::exception &e)
            {
                std::cerr << "error catched in " << __FILE__ << "::" << __LINE__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
            return return_seq;
        }

        /**
         * @brief behavior_nameの文字列を送信する前に辞書を参照して最小単位毎に数字に変える
         * @param[in] behavior_name 送信されるbehavior_nameの文字列
         * @return 辞書の対応する番号で構成される数字列
         * @details pyfilesディレクトリ内の辞書を参考にして数字に変換
         */
        SerializeStringByDict::ReturnNumSequenceType SerializeStringByDict::behaviorNameToNumSequence(const std::string &behavior_name)
        {
            std::string word;
            SerializeStringByDict::ReturnNumSequenceType return_seq;
            auto not_contain = behavior_name_to_num.end();
            try
            {
                for (const auto &character : behavior_name)
                {
                    word.push_back(character);
                    if (behavior_name_to_num.find(word) != not_contain)
                    {
                        return_seq.push_back(behavior_name_to_num[word]);
                        word.clear();
                    }
                }
            }
            catch (std::exception &e)
            {
                std::cerr << "error catched in " << __FILE__ << "::" << __LINE__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
            return return_seq;
        }

    }
}
