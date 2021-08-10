#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::udp;

class Server {
    asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    asio::streambuf receive_buff_;

public:
    Server(asio::io_service& io_service)
        : io_service_(io_service),
          socket_(io_service,udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 31400)) {}


private:

    // メッセージ受信
    void start_receive()
    {
        boost::asio::async_read(
            socket_,
            receive_buff_,
            asio::transfer_all(),
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

    server.start();

    io_service.run();
}