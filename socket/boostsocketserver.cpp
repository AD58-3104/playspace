#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class Server {
    asio::io_service& io_service_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;

public:
    Server(asio::io_service& io_service)
        : io_service_(io_service),
          acceptor_(io_service, tcp::endpoint(tcp::v4(), 31400)),
          socket_(io_service) {}

    void start_accept()
    {
        acceptor_.async_accept(
            socket_,
            boost::bind(&Server::on_accept, this, asio::placeholders::error));
    }

private:
    void on_accept(const boost::system::error_code& error)
    {
        if (error) {
            std::cout << "accept failed: " << error.message() << std::endl;
        }
        else {
            std::cout << "accept correct!" << std::endl;
        }
    }
};

int main()
{
    asio::io_service io_service;
    Server server(io_service);

    server.start_accept();

    io_service.run();
}