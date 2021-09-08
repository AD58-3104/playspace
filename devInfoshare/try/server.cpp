#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <thread>
#include <chrono>
#include <string>
namespace asio = boost::asio;
using asio::ip::udp;
using namespace std::literals::chrono_literals;

class InfoShareServer
{
    asio::io_service &io_service_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    bool terminated_ = false;
    // boost::array<char, 512> receive_buff_;
    boost::asio::streambuf receive_buff_;
    static const int32_t buffer_size_ = 1024;
    int32_t port_;

public:                     
    InfoShareServer(asio::io_service &io_service,int32_t port) //コンストラクタでRobotstatusの参照を渡しておく
        : io_service_(io_service),
          socket_(io_service, udp::endpoint(udp::v4(), port)),port_(port)
    {
        startReceive();
    }

    // メッセージ受信
    void startReceive()
    {
        socket_.async_receive_from(
            receive_buff_.prepare(buffer_size_),
            remote_endpoint_,
            // boost::bind(&InfoShareServer::receiveHandler, this,
            //             asio::placeholders::error, asio::placeholders::bytes_transferred)
            [this](const boost::system::error_code &error, size_t bytes_transferred){
                receiveHandler(error,bytes_transferred);
            }
            );
    }

    // 受信完了
    // error : エラー情報
    // bytes_transferred : 受信したバイト数
    void receiveHandler(const boost::system::error_code &error, size_t bytes_transferred)
    {
        if (error && error != boost::asio::error::eof)
        {
            std::cout << "receive failed: " << error.message() << std::endl;
        }
        else
        {
            // const char* data = asio::buffer_cast<const char*>(receive_buff_.data());
            const std::string data(boost::asio::buffer_cast<const char*>(receive_buff_.data()),bytes_transferred);
            // const std::string data(boost::asio::buffer_cast<const char*>(receive_buff_.data()),bytes_transferred);
            std::cout << data  << "::length " << bytes_transferred << std::endl;
            receive_buff_.consume(receive_buff_.size() + 1);
            if (data.compare("end") == 0)
            {
                terminated_ = true;
            }
            // receive_buff_.consume(receive_buff_.size());
            if (!terminated_)
            {
                startReceive();
            }
        }
    }
    void terminate(){
        io_service_.stop();
    };
};

int main()
{
    asio::io_service io_service;
    InfoShareServer server(io_service,7110);

    std::thread t([&io_service]
                  { io_service.run(); });
    std::this_thread::sleep_for(1000ms);
    t.join();
    // io_service.run();
}