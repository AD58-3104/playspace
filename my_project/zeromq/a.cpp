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
    publisher.bind("ipc://test_desu");
    int i = 0;
    while (true)
    {
        //  Write three messages, each with an envelope and content
        publisher.send(zmq::str_buffer("Message A::"), zmq::send_flags::sndmore);
        publisher.send(zmq::buffer(std::to_string(i)));
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        i++;
    }
}



int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    
    auto pubthread = std::async(std::launch::async, [&ctx](){Pub(ctx);});

    pubthread.wait();

    return 0;
}
