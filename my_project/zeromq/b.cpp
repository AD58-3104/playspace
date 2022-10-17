#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <array>

#include "zmq.hpp"
#include "zmq_addon.hpp"

void Sub(zmq::context_t &ctx)
{
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.connect("ipc:///tmp/hajime_quaternion");
    subscriber.set(zmq::sockopt::subscribe, "");
    std::array<uint8_t,512> data;
    zmq::mutable_buffer buf(data.data(),data.size());
    while (1)
    {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_buffer_result_t result = subscriber.recv(buf);//,zmq::recv_flags::dontwait);
        // assert(result && "recv failed"); // dame mitai
        // assert(*result == 2); // dame mitai
        std::cout << std::string(reinterpret_cast<char*>(buf.data())) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
    }
}

void Subbind(zmq::context_t &ctx)
{
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.bind("ipc:///tmp/connect");
    subscriber.set(zmq::sockopt::subscribe, "");
    constexpr size_t buf_size = 256; //byte
    std::array<uint8_t,buf_size> data;
    // void * buf_data = reinterpret_cast<void*>(malloc(buf_size));
    zmq::mutable_buffer buf(data.data(),data.size());
    // zmq::mutable_buffer buf(buf_data,buf_size);
    while (1)
    {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_buffer_result_t result = subscriber.recv(buf);//,zmq::recv_flags::dontwait);
        // assert(result && "recv failed"); // dame mitai
        // assert(*result == 2); // dame mitai
        std::cout << std::string(reinterpret_cast<char*>(buf.data())) << std::endl;
        data.fill(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
    }
}

int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    
    // auto subthread = std::async(std::launch::async, [&ctx](){Sub(ctx);});
    auto bindthread = std::async(std::launch::async, [&ctx](){Subbind(ctx);});

    // subthread.wait();
    bindthread.wait();

    return 0;
}
