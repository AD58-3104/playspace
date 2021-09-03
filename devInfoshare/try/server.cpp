#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <thread>
#include <string>
namespace asio = boost::asio;
using asio::ip::udp;

class Server
{
    asio::io_service &io_service_;
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    bool terminate = false;
    // boost::array<char, 512> receive_buff_;
    boost::asio::streambuf receive_buff_;

public:
    Server(asio::io_service &io_service)
        : io_service_(io_service),
          socket_(io_service, udp::endpoint(udp::v4(), 31400))
    {
        start_receive();
    }

    // メッセージ受信
    void start_receive()
    {
        socket_.async_receive_from(
            receive_buff_.prepare(512),
            remote_endpoint_,
            boost::bind(&Server::on_receive, this,
                        asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

    // 受信完了
    // error : エラー情報
    // bytes_transferred : 受信したバイト数
    void on_receive(const boost::system::error_code &error, size_t bytes_transferred)
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
                terminate = true;
            }
            // receive_buff_.consume(receive_buff_.size());
            if (!terminate)
            {
                start_receive();
            }
        }
    }
};

int main()
{
    asio::io_service io_service;
    Server server(io_service);

    std::thread t([&io_service]
                  { io_service.run(); });
    t.join();
    // io_service.run();
}