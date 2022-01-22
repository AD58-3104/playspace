#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include "zmq.hpp"
#include "zmq_addon.hpp"

void Sub(zmq::context_t &ctx)
{
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.connect("ipc://test_desu");
    subscriber.set(zmq::sockopt::subscribe, "");

    while (1)
    {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
            zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].to_string() << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    
    auto subthread = std::async(std::launch::async, [&ctx](){Sub(ctx);});

    subthread.wait();

    return 0;
}
