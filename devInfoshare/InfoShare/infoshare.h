#include <memory>
#include <vector>
#include <atomic>
#include <iostream>
#include <string>
#include <functional>
#include <boost/asio.hpp>

namespace Citbrains
{

    struct OtherRobotInfomation
    {
        std::atomic_uint32_t id;
        std::atomic_uint32_t cf_own;
        std::atomic_uint32_t cf_ball;
        std::atomic_uint32_t status;
        std::atomic_uint32_t fps;
        std::atomic_uint32_t voltage;
        std::atomic_uint32_t temperature;
        std::atomic_uint32_t highest_servo;
        std::atomic<std::string> command;
        std::atomic<std::string> current_behavior_name;
        // std::vector<std::atomic<object>> objects;        todo: protobufのメッセージ型にする。フラグ類は1つのbyteに詰め込む。型変換をしてからこっちに入れる。
    };

    class InfoShare
    {
    public:
        inline static constexpr int32_t NUM_PLAYERS = 6;
        inline static constexpr int32_t COMM_INFO_PORT0 = 7110; //!< CommInfoで使用するPORTのはじめのポート
                                              //1:7110, 2:7111, 3:7112, 4:7113, 5:7114, 6:7115

        inline static constexpr int32_t MAX_COMM_INFO_OBJ = 7;  //!< 共有するオブジェクトの最大値(はじめはボール)
        inline static constexpr int32_t MAX_STRING = 42;        //!< メッセージの最大値
        inline static constexpr int32_t MAX_BEHAVIOR_STRING = 32;       //この三つは多分もういらない

        inline static constexpr uint8_t COMM_NOT_EXIST = 0b0000'0000;     //!< 発見しないときの値
        inline static constexpr uint8_t COMM_EXIST = 0b0000'0001;         //!< 発見しかたどうかのフラグ
        inline static constexpr uint8_t COMM_OUR_SIDE = 0b0000'0010;      //!< 自チームかどうかのフラグ
        inline static constexpr uint8_t COMM_OPPOSITE_SIDE = 0b0000'0100; //!< 敵チームかどうかのフラグ

        inline static constexpr int32_t MAX_BLACK_POLES = 6; //!< 黒ポールの数
        inline static constexpr int32_t MAX_TARGET_POS = 1;
        inline static constexpr int32_t MAX_MAGENTA_OBJECTS = 3;                //!< マゼンタロボットの数
        inline static constexpr int32_t MAX_CYAN_OBJECTS = MAX_MAGENTA_OBJECTS; //!< シアンロボットの数

        inline static constexpr int32_t NO_MAGENTA = 0; //!< マゼンタの番号(色の番号とは異なる)
        inline static constexpr int32_t NO_CYAN = 1;    //!< シアンの番号
//---------------------------------------------------------------------------
        [[nodiscard]] getter(const int id) const noexcept;//atomicなloadをしたのを返す。
//---------------------------------------------------------------------------
        void terminate(void);
        bool sendInfomationToOtherRobots();
        void changeColor(const int32_t id, const int32_t color);
        void setTimeFunc(float (*func)());
        float getTime(void);

    private:
        std::vector<std::unique_ptr<Citbrains::otherRobotInfomation>> robot_data_list_;
        MessageProto::SharingData sharing_data_;
        void receiveSharedInfomation();

        void receivingThreadLoop();
        void sendingThreadLoop();
    };

}