#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include <string>
#include <functional>
#include <boost/asio.hpp>
#include "hpl_types.h"

namespace Citbrains
{
    namespace infosharemodule
    {
        struct OtherRobotInfomation
        {
            enum class MutexNumber{
                COMMAND = 0,
                CURRENT_BEHAVIOR_NAME = 1,
                OUR_ROBOT_GL = 2,
                ENEMY_ROBOT_GL = 3,
                BLACK_POLE_GL = 4,
                TARGET_POS_VEC = 5,
                LENGTH  //最大数を知りたい時に使うので何か追加する時はこれの前に追加する。
            }
            //scoped_lockして
            std::vercor<std::mutex> mutexes_;
            OtherRobotInfomation(){
                for(size_t i = 0;i < static_cast<size_t>(MutexNumber::LENGTH);i++){
                    mutexes_.emplace_back(std::mutex());
                }
            }
            private:
            std::atomic_uint32_t id_;
            std::atomic_uint32_t cf_own_;
            std::atomic_uint32_t cf_ball_;                  //fastにした方が良いのかもしれない
            std::atomic_uint32_t status_;
            std::atomic_uint32_t fps_;
            std::atomic_uint32_t voltage_;
            std::atomic_uint32_t temperature_;
            std::atomic_uint32_t highest_servo_;
            std::atomic_bool is_detect_ball_;
            std::atomic_uint32_t strategy_no_;
            std::atomic<float> recv_time_;

            std::string command_;
            std::string current_behavior_name_;

            std::vector<Pos2D> our_robot_gl_;  //扱いが良く分からんやつら
            std::vector<Pos2D> enemy_robot_gl_;
            std::vector<Pos2D> black_pole_gl_;
            std::vector<Pos2D> target_pos_vec_;

        };

        class InfoShare
        {
        public:
            //inline変数を使っているのでc++17以上でしかコンパイル出来ない。
            #ifdef  __cpp_inline_variables
            inline static constexpr int32_t NUM_PLAYERS = 6;
            inline static constexpr int32_t COMM_INFO_PORT0 = 7110; //!< CommInfoで使用するPORTのはじめのポート
                                                                    //1:7110, 2:7111, 3:7112, 4:7113, 5:7114, 6:7115

            inline static constexpr int32_t MAX_COMM_INFO_OBJ = 7;    //!< 共有するオブジェクトの最大値(はじめはボール)
            inline static constexpr int32_t MAX_STRING = 42;          //!< メッセージの最大値
            inline static constexpr int32_t MAX_BEHAVIOR_STRING = 32; 

            inline static constexpr uint8_t COMM_NOT_EXIST = 0b0000'0000;     //!< 発見しないときの値　もう必要なさそうhasを使えば良いし
            inline static constexpr uint8_t COMM_EXIST = 0b0000'0001;         //!< 発見しかたどうかのフラグ
            inline static constexpr uint8_t COMM_OUR_SIDE = 0b0000'0010;      //!< 自チームかどうかのフラグ
            inline static constexpr uint8_t COMM_OPPOSITE_SIDE = 0b0000'0100; //!< 敵チームかどうかのフラグ

            inline static constexpr int32_t MAX_BLACK_POLES = 6; //!< 黒ポールの数
            inline static constexpr int32_t MAX_TARGET_POS = 1;
            inline static constexpr int32_t MAX_MAGENTA_OBJECTS = 3;                //!< マゼンタロボットの数
            inline static constexpr int32_t MAX_CYAN_OBJECTS = MAX_MAGENTA_OBJECTS; //!< シアンロボットの数

            inline static constexpr int32_t NO_MAGENTA = 0;    //!< マゼンタの番号(色の番号とは異なる)
            inline static constexpr int32_t NO_CYAN = 1;       //!< シアンの番号
            #else
            //jetsonで無理だったら実装する.書くと長すぎて見にくいので.
            #endif
            InfoShare(const int32_t self_id,const int32_t our_color,const std::string ip_address,float (*timeFunc)() );
            ~InfoShare();
            //---------------------------------------------------------------------
            //atomicなloadをしたのを返す一連のgetter。
            [[nodiscard]] int32_t get_cf_own(const int& id) const noexcept; 
            [[nodiscard]] std::string get_command(const int& id) const ; //残念ながらunique_lockは例外を投げるらしい
            //---------------------------------------------------------------------
            void terminate(void);
            bool sendInfomationToOtherRobots();
            void changeColor(const int32_t id, const int32_t color);
            void setTimeFunc(float (*func)());
            float getTime(void);

        private:
            std::vector<std::unique_ptr<Citbrains::infosharemodule::OtherRobotInfomation>> robot_data_list_;
            MessageProto::SharingData sharing_data_;
            std::thread receive_thread_;
            std::thread send_thread_;
            const int32_t self_id_;
            bool terminated_ = false;
            void receiveSharedInfomation();
            void receivingThreadLoop();
            void sendingThreadLoop();
        };

    }
}