#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <future>

using namespace std::literals;
bool terminate = false;
void f(zmq::socket_t &sock)
{
    try
    {

        for (;!terminate;)
        {
            sock.send(zmq::str_buffer("hello world"), zmq::send_flags::none);
            // std::cout << "send" << std::endl;
            std::this_thread::sleep_for(1ns);
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }
}

size_t pub_n = 10;
int main(int argc, char const *argv[])
{
    std::string s("ipc://aaa");
    zmq::context_t ctx;
    std::vector<zmq::socket_t> pubs;
    std::vector<std::future<void>> threads;
    for (size_t i = 0; i < pub_n; ++i)
        pubs.emplace_back(ctx, zmq::socket_type::pub);
    int x = 0;
    for (auto &itm : pubs)
    {
        itm.bind(s + std::to_string(x));
        x++;
    }
    zmq::socket_t sub(ctx, zmq::socket_type::sub);
    for (size_t i = 0; i < pub_n; ++i)
        sub.connect(s + std::to_string(i));
    sub.set(zmq::sockopt::subscribe, "");
    std::this_thread::sleep_for(20ms);
    for (size_t i = 0; i < pub_n; ++i)
    {
        threads.emplace_back(std::async(std::launch::async,f,std::ref(pubs[i])));
    }
    int64_t cnt = 0;
    try
    {

        for (;;)
        {
            zmq::message_t msg;
            if (sub.recv(&msg))
            {
                cnt++;
                std::cout << msg.to_string() << std::endl;
            }
            if (cnt > 1'00'000)
            {
                std::cout << "-----------------end----------------------" << std::endl;
                terminate = true;
                for(auto& itm:pubs){
                    itm.close();
                }
                for(auto& itm:threads){
                    itm.get();
                }
                sub.close();
                ctx.close();
                ctx.shutdown();
                return 0;
            }
        }
    }
    catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
