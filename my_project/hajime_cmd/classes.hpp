#include <iostream>
#include <vector>
#include <array>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>
#include <optional>
#include <atomic>

/*todo list
    例外の伝播をどうするかを考える

*/


namespace Citbrains
{
    namespace walkCommunication
    {

        static const std::string robot_state_topic("ipc:///tmp/robot_quaternion");
        static const std::string command_topic("ipc:///tmp/robot_command");
        enum class option
        {
            block,
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
            void send_state(std::string &&data, option op = option::nonblock)
            {
                if (op == option::block)
                {
                    zmq::send_result_t result = pub_.send(zmq::buffer(data));
                }
                else
                {
                    zmq::send_result_t result = pub_.send(zmq::buffer(data), zmq::send_flags::dontwait);
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
            using proto_state_t = void; // robotStatusを返すべきかも
            using proto_command_t = void;
            walkCommunicationClient():suspended_(false),ctx_(),pub_(ctx_, zmq::socket_type::pub),sub_(ctx_, zmq::socket_type::sub){
                pub_.connect(command_topic);
                sub_.connect(robot_state_topic);
                sub_.set(zmq::sockopt::subscribe,"");
            }
            proto_state_t get_quaternion() //ここ分ける必要無さそうな気もする。
            {
            }
            proto_state_t get_state() //こちらだけで十分では
            {
            }
            void hajime_walk(int num, int angle, int stridex, int period, int stridey);
            void hajime_accurate_walk(int num, float x, float y, float th); // x[mm], y[mm], th[deg]
            void hajime_accurate_one_step(float x, float y, float th);      // x[mm], y[mm], th[deg]
            void hajime_cancel();
            void hajime_pan(int pan, int time);
            void hajime_tilt(int tilt, int time);
            void hajime_motion(int no, int repeat, bool ignoresuspend = false);
            void hajime_variable_motion(int no, int shift);
            void hajime_pantilt(int pan, int tilt, int time);
            void hajime_power(int OnOff);
            void hajime_set_suspended(bool suspend);
            float getVoltage();
            proto_state_t getStatus();
            proto_state_t getStatusQuaternion();

        private:
            std::atomic_bool suspended_;
            inline static constexpr size_t buf_size_ = 512;
            zmq::context_t ctx_;
            zmq::socket_t pub_;
            zmq::socket_t sub_;
            /**
             * @brief マルチスレッドで呼ばないなら安全.そんな事するくらいならbuf_dataいちいち作る方が良いかな...。
             *
             * @param op
             * @return std::string
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
             * @brief swigを通すと例外を伝播出来ないっぽいので
             * 
             * @param data 
             * @param op 
             */
            void send_command(string&& data, option op = option::nonblock)
            {
                if (op == option::block)
                {
                    zmq::send_result_t result = pub_.send(zmq::buffer(data));
                }
                else
                {
                    zmq::send_result_t result = pub_.send(zmq::buffer(data), zmq::send_flags::dontwait);
                }
            };
        };
    }
}