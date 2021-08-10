#include "infoshare.h"
#include <array>
using namespace chrono_literals;

Citbrains::infosharemodule::InfoShare(const int32_t self_id,const int32_t our_color,const std::string ip_address,float (*timeFunc)() )
:self_id_(self_id),terminated(false),
{

}
Citbrains::infosharemodule::~InfoShare()
{
    terminate();
}

Citbrains::infosharemodule::receiveSharedInfomation(){
    while(!terminated_){
        try
        {
            boost::asio::io_service io_service;                 //毎回構築していいのかしら。
            boost::asio::ip::udp::socket socket(io_service,boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),COMM_INFO_PORT0 + self_id_) );
            std::array<std::byte,1024> receive_buffer;
            while(!terminated_){
                if(!socket.available()){
                    std::this_thread::sleep_for(50ms);
                    continue;
                }
                boost::system::error_code error;
                boost::asio::ip::udp::endpoint remote_endpoint;
                size_t receive_length = socket.receive_from(boost::asio::buffer(receive_buffer),remote_endpoint,0,error);
                if(error)continue;
                if()            //TODO:ここでハッシュのチェックでもする？？ダメだったら落とすみたいな


            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        
    }
}