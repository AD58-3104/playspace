#include <iostream>
#include <vector>
#include <array>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>

namespace Citbrains
{
    namespace walkCommunication
    {
        static const std::string robot_state_topic("ipc:///tmp/robot_quaternion");
        static const std::string command_topic("ipc:///tmp/robot_command");
        enum class option
        {
            block;
            nonblock
        };
        /**
         * @brief recvとsendが両方ともアトミックな操作だと限定出来るならばbuf_data_を状態として良い.
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
             * @brief マルチスレッドで呼ばないなら安全.そんな事するくらいならbuf_dataいちいち作る方が良いかな...。
             *
             *
             * @param op
             * @return std::string
             */

            std::string recv_command(option op = option::block)
            {
                // todo これで平気かな？？逆に作りまくられたらどうしよう...
                thread_local static std::array<uint8_t, buf_size> buf_data;
                zmq::mutable_buffer buf(buf_data.data(), buf_data.size());

                if (op == option::block)
                {
                    zmq::recv_buffer_result_t result = sub_.recv(buf); //,zmq::recv_flags::dontwait);
                }
                else
                {
                    zmq::recv_buffer_result_t result = sub_.recv(buf, zmq::recv_flags::dontwait);
                }
                buf_data.fill(0);
            };
            void send_state(std::string data);

        private:
            zmq::context_t ctx_;
            constexpr size_t buf_size_ = 256;
            zmq::socket_t pub_;
            zmq::socket_t sub_;
        };
        class walkCommunicationClient
        {
        };
    };
};