#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include "zmq.hpp"
#include "zmq_addon.hpp"

void Pub(zmq::context_t &ctx)
{
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    publisher.connect("tcp://172.29.224.1:9544");
    int i = 123;
    while (true)
    {
        //  Write three messages, each with an envelope and content
        // publisher.send(zmq::str_buffer("Message A::"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer("Pub2 message::" + std::to_string(i)));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        i++;
    }
}

void req(zmq::context_t &ctx)
{
    zmq::socket_t publisher(ctx, zmq::socket_type::req);
    publisher.connect("tcp://172.29.224.1:9544");
    int i = 1;
    while (true)
    {
        //  Write three messages, each with an envelope and content
        // publisher.send(zmq::str_buffer("Message A::"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer("receive message::" + std::to_string(i)));
        std::cout << "send " << i << std::endl;
        std::array<char, 1024> recv_data;
        recv_data.fill(static_cast<char>(0));
        auto buf = zmq::buffer(recv_data.data(), recv_data.size());
        publisher.recv(buf);
        std::cout << std::string(reinterpret_cast<char*>(buf.data())) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        i++;
    }
}


int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    
    // auto pubthread = std::async(std::launch::async, [&ctx](){Pub(ctx);});
    auto reqthread = std::async(std::launch::async, [&ctx](){req(ctx);});

    // pubthread.wait();
    reqthread.wait();

    return 0;
}
