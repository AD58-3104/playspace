#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::udp;

class Server {
    asio::io_service& io_service_;
    // udp::acceptor acceptor_;
    udp::socket socket_;
    asio::streambuf receive_buff_;
    udp::endpoint remote_endpoint_;

public:
    Server(asio::io_service& io_service)
        : io_service_(io_service),
        socket_(io_service,udp::endpoint(udp::v4(), 31400))
        {
            start_receive();
        }

    // メッセージ受信
    void start_receive()
    {
        socket_.async_receive_from(
            receive_buff_,
            remote_endpoint_,
            asio::transfer_at_least(1),
            boost::bind(&Server::on_receive, this,
                        asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

    // 受信完了
    // error : エラー情報
    // bytes_transferred : 受信したバイト数
    void on_receive(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (error && error != boost::asio::error::eof) {
            std::cout << "receive failed: " << error.message() << std::endl;
        }
        else {
            const char* data = asio::buffer_cast<const char*>(receive_buff_.data());
            std::cout << data << std::endl;

            receive_buff_.consume(receive_buff_.size());
        }
    }
};

int main()
{
    asio::io_service io_service;
    Server server(io_service);



    io_service.run();
}