#include "classes.hpp"
namespace Citbrains
{
    namespace walkCommunication
    {
        // noexceptの変わりに失敗した時はログを残す。
        void walkCommunicationClient::hajimeWalk(int num, int angle, int stridex, int period, int stridey) noexcept
        {
        }
        void walkCommunicationClient::hajimeAccurateWalk(int num, float x, float y, float th) noexcept
        {

        } // x[mm], y[mm], th[deg]
        void walkCommunicationClient::hajimeAccurateOneStep(float x, float y, float th) noexcept
        {

        } // x[mm], y[mm], th[deg]
        void walkCommunicationClient::hajimeCancel() noexcept
        {
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::CANCEL);
            send_command(com.SerializeAsString());

        }
        void walkCommunicationClient::hajimePan(int pan, int time) noexcept
        {
        }
        void walkCommunicationClient::hajimeTilt(int tilt, int time) noexcept
        {
        }
        void walkCommunicationClient::hajimeMotion(int no, int repeat, bool ignoresuspend) noexcept
        {
        }
        void walkCommunicationClient::hajimeVariableMotion(int no, int shift) noexcept
        {
        }
        void walkCommunicationClient::hajimePanTilt(int pan, int tilt, int time) noexcept
        {
        }
        void walkCommunicationClient::hajimePower(bool OnOff) noexcept
        {
            
        }
        void walkCommunicationClient::hajimeSetSuspended(bool suspend) noexcept
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