#include "classes.hpp"
#include <syslog.h>

void logDummy(std::string s){
    syslog(LOG_ALERT,"%s",s.c_str());
};

const std::string SEND_FAILED("Command send failed. ZMQ send queue is full.");

namespace Citbrains
{
    namespace walkCommunication
    {
        // noexceptの変わりに失敗した時はログを残す。
        void walkCommunicationClient::hajimeWalk(int num, int angle, int stridex, int period, int stridey) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::WALK);
            com.mutable_walk_args()->set_num(num);
            com.mutable_walk_args()->set_angle(angle);
            com.mutable_walk_args()->set_stridex(stridex);
            com.mutable_walk_args()->set_period(period);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimeAccurateWalk(int steps, float x, float y, float th) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::ACCURATE_WALK);
            com.mutable_accurate_walk_args()->set_steps(steps);
            com.mutable_accurate_walk_args()->set_x(x);
            com.mutable_accurate_walk_args()->set_y(y);
            com.mutable_accurate_walk_args()->set_th(th);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        } // x[mm], y[mm], th[deg]
        void walkCommunicationClient::hajimeAccurateOneStep(float x, float y, float th) noexcept
        {
            if (is_suspended_.load())
                return;
            walkCommunicationClient::hajimeAccurateWalk(1, x, y, th);
        } // x[mm], y[mm], th[deg]

        void walkCommunicationClient::hajimeCancel() noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::CANCEL);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimePan(int pan, int time) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::PAN);
            com.mutable_pan_args()->set_pan(pan);
            com.mutable_pan_args()->set_time(time);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimeTilt(int tilt, int time) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::TILT);
            com.mutable_tilt_args()->set_tilt(tilt);
            com.mutable_tilt_args()->set_time(time);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimeMotion(int no, int repeat, bool ignoresuspend) noexcept
        {
            if (is_suspended_.load() && !ignoresuspend)
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::MOTION);
            com.mutable_motion_args()->set_no(no);
            com.mutable_motion_args()->set_repeat(repeat);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimePanTilt(int pan, int tilt, int time) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::PANTILT);
            com.mutable_pantilt_args()->set_pan(pan);
            com.mutable_pantilt_args()->set_tilt(tilt);
            com.mutable_pantilt_args()->set_time(time);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimePower(bool OnOff) noexcept
        {
            if (is_suspended_.load())
                return;
            ControlMessage::WalkCommand com;
            com.set_command_num(ControlMessage::WalkCommand::POWER);
            com.mutable_power_args()->set_on(OnOff);
            try
            {
                if (size_t size = send_command(com.SerializeAsString()); size < 1)
                {
                    logDummy(SEND_FAILED);
                }
            }
            catch (std::exception &e)
            {
                logDummy(e.what());
            }
        }
        void walkCommunicationClient::hajimeSetSuspended(bool suspend) noexcept
        {
            is_suspended_.store(suspend);
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