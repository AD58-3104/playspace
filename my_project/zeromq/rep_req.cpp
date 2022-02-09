#include "zmq.hpp"
#include <string>
#include <iostream>
#include <future>
#include <array>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <unistd.h>
int main(int argc, char const *argv[])
{
    zmq::context_t ctx;
    zmq::socket_t request(ctx, zmq::socket_type::req);
    zmq::socket_t reply(ctx, zmq::socket_type::rep);
    const std::string s("ipc://reqrep");
    std::array<std::byte, 1024> buf;
    static_assert(buf.max_size() == 1024, "noo");
    reply.bind(s);
    request.connect(s);
    auto req = std::async(std::launch::async, [&]()
                          {
                            std::array<std::byte,1024> data;
                            data.fill(static_cast<std::byte>(0));
                            auto b = zmq::buffer(data.data(),data.size());
                            request.send(zmq::str_buffer("REQ")); 
                            request.recv(b); 
                            std::string str((const char*)(b.data()));
                            std::cout << str << " received on require socket" << std::endl; });
    auto rep = std::async(std::launch::async, [&]()
                          {
                            usleep(100000);
                            std::array<std::byte,1024> data;
                            data.fill(static_cast<std::byte>(0));
                            auto b = zmq::buffer(data.data(),data.size());
                            reply.recv(b);
                            std::cout  << std::string((const char *)(b.data())) << " received on reply socket" << std::endl;
                            sleep(1);
                            reply.send(zmq::str_buffer("REP")); });
    return 0;
}
