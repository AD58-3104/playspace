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
    publisher.bind("ipc:///tmp/hajime_quaternion");
    int i = 0;
    while (true)
    {
        //  Write three messages, each with an envelope and content
        // publisher.send(zmq::str_buffer("Message A::"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer("Pub1!!!!!!!!! message::" + std::to_string(i)));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        i++;
    }
}

void PubConnect(zmq::context_t &ctx)
{
    zmq::socket_t pub(ctx, zmq::socket_type::pub);
    pub.connect("ipc:///tmp/connect");
    int i = 0;
    while (true)
    {
        //  Write three messages, each with an envelope and content
        // pub.send(zmq::str_buffer("Message A::"), zmq::send_flags::sndmore);
        pub.send(zmq::buffer("Pub1!!!!!!!!! message::" + std::to_string(i)));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        i++;
    }
}


int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    std::cout << "start" << std::endl;
    zmq::socket_t   socket(ctx, zmq::socket_type::pub);
    zmq::socket_ref ref = socket;
    if(ctx.handle() != nullptr){
        std::cout << "ctx is valid" << std::endl;
    }
    else{
        std::cout << "ctx is invalid" << std::endl;
    }
    std::cout << "shutdown" << std::endl;
    ctx.shutdown();
    if(ctx.handle() != nullptr){
        std::cout << "ctx is valid" << std::endl;
    }
    else{
        std::cout << "ctx is invalid" << std::endl;
    }
    // std::cout << "close" << std::endl;
    // ctx.close();
    // if(ctx.handle() != nullptr){
    //     std::cout << "ctx is valid" << std::endl;
    // }
    // else{
    //     std::cout << "ctx is invalid" << std::endl;
    // }
    ref.close();
    // auto pubthread = std::async(std::launch::async, [&ctx](){Pub(ctx);});
    // auto subthread = std::async(std::launch::async, [&ctx](){PubConnect(ctx);});

    // pubthread.wait();
    // subthread.wait();
    return 0;
}
