#include "classes.hpp"
namespace Citbrains
{
    namespace walkCommunication
    {
        // noexceptの変わりに失敗した時はログを残す。
        void walkCommunicationClient::hajime_walk(int num, int angle, int stridex, int period, int stridey) noexcept
        {
        }
        void walkCommunicationClient::hajime_accurate_walk(int num, float x, float y, float th) noexcept
        {

        } // x[mm], y[mm], th[deg]
        void walkCommunicationClient::hajime_accurate_one_step(float x, float y, float th) noexcept
        {

        } // x[mm], y[mm], th[deg]
        void walkCommunicationClient::hajime_cancel() noexcept
        {
        }
        void walkCommunicationClient::hajime_pan(int pan, int time) noexcept
        {
        }
        void walkCommunicationClient::hajime_tilt(int tilt, int time) noexcept
        {
        }
        void walkCommunicationClient::hajime_motion(int no, int repeat, bool ignoresuspend) noexcept
        {
        }
        void walkCommunicationClient::hajime_variable_motion(int no, int shift) noexcept
        {
        }
        void walkCommunicationClient::hajime_pantilt(int pan, int tilt, int time) noexcept
        {
        }
        void walkCommunicationClient::hajime_power(int OnOff) noexcept
        {
        }
        void walkCommunicationClient::hajime_set_suspended(bool suspend) noexcept
        {
        }
        std::optional<float> walkCommunicationClient::getVoltage() noexcept
        {
            ControlMessage::RobotStatus status;
            if (auto byte_str = recv_state(); byte_str.has_value())
            {
                status.ParseFromString(byte_str.value());
                return status.motorvoltage();
            }
            else
            {
                // TODO:logging
                return std::nullopt;
            }
        }
        std::optional<walkCommunicationClient::RobotStatus_t> walkCommunicationClient::getStatus() noexcept
        {
            ControlMessage::RobotStatus status;
            if (auto byte_str = recv_state(); byte_str.has_value())
            {
                status.ParseFromString(byte_str.value());
                return status;
            }
            else
            {
                // TODO:logging
                return std::nullopt;
            }
        }
        std::optional<walkCommunicationClient::RobotStatus_t> walkCommunicationClient::getStatusQuaternion() noexcept
        {
            ControlMessage::RobotStatus status;
            if (auto byte_str = recv_state(); byte_str.has_value())
            {
                status.ParseFromString(byte_str.value());
                return status;
            }
            else
            {
                // TODO:logging
                return std::nullopt;
            }
        } //こっちはいらんだろう
    }
}