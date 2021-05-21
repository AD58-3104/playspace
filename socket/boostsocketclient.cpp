#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class Client {
    asio::io_service& io_service_;
    tcp::socket socket_;

public:
    Client(asio::io_service& io_service)
        : io_service_(io_service),
          socket_(io_service)
    {}

    void connect()
    {
        socket_.async_connect(
                tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 31400),
                boost::bind(&Client::on_connect, this, asio::placeholders::error));
    }

    void on_connect(const boost::system::error_code& error)
    {
        if (error) {
            std::cout << "connect failed : " << error.message() << std::endl;
        }
        else {
            std::cout << "connected" << std::endl;
        }
    }
};

int main()
{
    asio::io_service io_service;
    Client client(io_service);

    client.connect();

    io_service.run();
}