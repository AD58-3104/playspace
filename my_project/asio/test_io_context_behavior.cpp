#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>
using namespace std::literals::chrono_literals;

std::string s;
std::atomic_uint32_t call_cnt = 0;

void print(const boost::system::error_code& /*e*/)
{
    std::this_thread::sleep_for(5ms);
    // std::cout << "Hello, world!" << std::endl;
    s += "print ";
    call_cnt++;
}

int main(int argc, char const *argv[])
{
    boost::asio::io_context io;
    size_t cnt = 0;
    // while(true){
    //     std::cout << "start" << std::endl;
    //     boost::asio::steady_timer t(io, 300ms);
    //     t.async_wait(&print);
    //     std::cout << "executed " << io.run()  <<std::endl;
    //     if(io.stopped()){
    //         std::cout << "io stopped" << std::endl;
    //     }
    //     else{
    //         std::cout << "-------- io not stopped --------" << std::endl;
    //     }
    //     if(cnt > 5){               //ここでrestartされるまでは、executorは実行されない。
    //         // io.stop();              //stopされたら単にexecutorの実行が止まるだけで、ポストされたものが無くなる訳ではない。
    //         std::cout << "---stop---" << std::endl;
    //         std::this_thread::sleep_for(2s);
    //         io.restart();
    //         io.run();
    //         std::cout << "restarted" << std::endl;
    //     }
    //     std::this_thread::sleep_for(500ms);
    //     cnt++;
    // }
    std::cout << "start" << std::endl;
    while(true){
        boost::asio::steady_timer t(io, 100s);
        t.async_wait(&print);
        t.cancel();             //cancelするとすぐさまハンドラがinvocation_queueに乗る
        io.run();
        if(cnt > 1000){               //ここでrestartされるまでは、executorは実行されない。
            std::cout << "--- call --- " << io.stopped() << std::endl;
            io.restart();
            std::cout << "restarted " << io.stopped() << std::endl;
            io.restart();
            // std::this_thread::sleep_for(1s);
            std::cout <<  " start call_cnt: " << call_cnt << std::endl;
            std::cout << "handler " <<io.run() << std::endl;
            std::this_thread::sleep_for(1s);
            std::cout <<  " final call_cnt: " << call_cnt << std::endl;
            return 0;
        }
        cnt++;
    }
    return 0;
}

