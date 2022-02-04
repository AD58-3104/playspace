#ifndef WALK_COMMUNICATOR_H_
#define WALK_COMMUNICATOR_H_
#include <iostream>
#include <vector>
#include <array>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>
#include <optional>
#include <atomic>
#include "hajime.pb.h"

/*todo list memo
    例外の伝播をどうするかを考える ->

*/

#ifndef ZMQ_CPP17
static_assert(false, "------------------------------cppzmq version is old!!!!------------------------------")
#endif // ZMQ_CPP17

    namespace Citbrains
{
    namespace walkCommunication
    {
        // struct RobotStatus
        // {
        //     int32_t type;
        //     int32_t Active;
        //     int32_t Command;
        //     int32_t Posture;
        //     int32_t Temperature;
        //     int32_t servo1temp;
        //     int32_t MotorVoltage;
        //     int32_t Odometry[3];
        //     float gyroMotion; // rad
        //     int32_t Servo[STATE_MOTOR_NUM];
        //     float servo_rad[STATE_MOTOR_NUM];
        //     float quaternion[4];
        // };

        static const std::string robot_state_topic("ipc:///tmp/robot_quaternion");
        static const std::string command_topic("ipc:///tmp/robot_command");
        enum class option
        {
            block,
            nonblock
        };

        /**
         * @brief
         * @todo recvとsendが両方ともアトミックな操作だと限定出来るならばbuf_data_を状態として良い.
         *         正直それやるより素直に毎度構築する方がマシ。lockとかの方が遙かにコストが高そう.
         */
        class walkCommunicationServer
        {
        public:
            walkCommunicationServer() : ctx_(), pub_(ctx_, zmq::socket_type::pub), sub_(ctx_, zmq::socket_type::sub)
            {
                pub_.bind(robot_state_topic);
                sub_.bind(command_topic);
                sub_.set(zmq::sockopt::subscribe, "");
            };

            /**
             * @brief ブロッキングする。
             * @exception zmq::error_t  cppzmqの例外.whatでconst char*を得られる
             * @return 受信したバイト列。失敗時はnulloptが返る
             * @todo マルチスレッドで呼んでも安全にする
             */
            std::optional<std::string> recv_command()
            {
                // todo これで平気かな？？逆に作りまくられたらどうしよう...
                thread_local static std::array<uint8_t, buf_size_> buf_data;
                buf_data.fill(0);
                zmq::mutable_buffer buf(buf_data.data(), buf_data.size());
                zmq::recv_buffer_result_t result = sub_.recv(buf);
                if (result.has_value())
                {
                    if (result->truncated())
                    {
                        return std::nullopt;
                    }
                    return std::optional<std::string>(std::string(reinterpret_cast<char *>(buf.data()), result->size));
                }
                return std::nullopt;
            };

            /**
             * @brief swigを通すと例外を伝播出来ないっぽいのでこれの一つ上でハンドルする
             *
             * @param data 送るデータ
             * @param op ブロッキングか否か
             * @exception zmq::error_t  cppzmqの例外.whatでconst char*を得られる
             * @return 送った文字数.0の場合はソケットのキューが満杯で送れない時.
             */
            size_t send_state(std::string &&data, option op = option::nonblock)
            {
                zmq::send_result_t result;
                if (op == option::block)
                {
                    result = pub_.send(zmq::buffer(data), zmq::send_flags::none);
                }
                else
                {
                    result = pub_.send(zmq::buffer(data), zmq::send_flags::dontwait);
                }
                if (result.has_value())
                {
                    return 0;
                }
                else
                {
                    return result.value();
                }
            };

        private:
            zmq::context_t ctx_;
            inline static constexpr size_t buf_size_ = 256;
            zmq::socket_t pub_;
            zmq::socket_t sub_;
        };

        /**
         * @brief こっちはユーザ側で使う事が多いのでprotobufのパースまでこちらでやる。
         * コンテキストが沢山作られるのが嫌なので仕方無く大量に関数を定義します。
         */
        class walkCommunicationClient
        {
        public:
            using proto_q_t = void;
            using RobotStatus_t = ControlMessage::RobotStatus; // robotStatusを返すべきかも
            using proto_command_t = void;
            walkCommunicationClient() : suspended_(false), ctx_(), pub_(ctx_, zmq::socket_type::pub), sub_(ctx_, zmq::socket_type::sub)
            {
                pub_.connect(command_topic);
                sub_.connect(robot_state_topic);
                sub_.set(zmq::sockopt::subscribe, "");
            }
            // noexceptの変わりに失敗した時はログを残す。
            void hajimeWalk(int num, int angle, int stridex, int period, int stridey) noexcept;
            void hajimeAccurateWalk(int num, float x, float y, float th) noexcept; // x[mm], y[mm], th[deg]
            void hajimeAccurateOneStep(float x, float y, float th) noexcept;       // x[mm], y[mm], th[deg]
            void hajimeCancel() noexcept;
            void hajimePan(int pan, int time) noexcept;
            void hajimeTilt(int tilt, int time) noexcept;
            void hajimeMotion(int no, int repeat, bool ignoresuspend = false) noexcept;
            void hajimeVariableMotion(int no, int shift) noexcept;
            void hajimePanTilt(int pan, int tilt, int time) noexcept;
            void hajimePower(bool OnOff) noexcept;
            void hajimeSetSuspended(bool suspend) noexcept;
            std::optional<float> getVoltage() noexcept;
            std::optional<RobotStatus_t> getStatus() noexcept;
            std::optional<RobotStatus_t> getStatusQuaternion() noexcept;

        private:
            std::atomic_bool suspended_;
            inline static constexpr size_t buf_size_ = 512;
            zmq::context_t ctx_;
            zmq::socket_t pub_;
            zmq::socket_t sub_;
            /**
             * @brief ブロッキングする。
             * @exception zmq::error_t  cppzmqの例外.whatでconst char*を得られる
             * @return 受信したバイト列。失敗時はnulloptが返る
             * @todo マルチスレッドで呼んでも安全にする
             */
            std::optional<std::string> recv_state()
            {
                // todo これで平気かな？？逆に作りまくられたらどうしよう...
                thread_local static std::array<uint8_t, buf_size_> buf_data;
                buf_data.fill(0);
                zmq::mutable_buffer buf(buf_data.data(), buf_data.size());
                zmq::recv_buffer_result_t result = sub_.recv(buf);
                if (result.has_value())
                {
                    if (result->truncated())
                    {
                        return std::nullopt;
                    }
                    return std::optional<std::string>(std::string(reinterpret_cast<char *>(buf.data()), result->size));
                }
                return std::nullopt;
            };
            /**
             * @brief swigを通すと例外を伝播出来ないっぽいのでこれの一つ上でハンドルする
             *
             * @param data 送るデータ
             * @param op ブロッキングか否か
             * @exception zmq::error_t  cppzmqの例外.whatでconst char*を得られる
             * @return 送った文字数.0の場合はソケットのキューが満杯で送れない時.
             */
            size_t send_command(std::string &&data, option op = option::nonblock)
            {
                zmq::send_result_t result;
                if (op == option::block)
                {
                    result = pub_.send(zmq::buffer(data), zmq::send_flags::none); // return recv size
                }
                else
                {
                    result = pub_.send(zmq::buffer(data), zmq::send_flags::dontwait);
                }
                if (result.has_value())
                {
                    return 0;
                }
                else
                {
                    return result.value();
                }
            };
        };
    }
}
#endif // !WALK_COMMUNICATOR_H_