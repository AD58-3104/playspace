#include "protobuf_msg_worker.hpp"

int main(int argc, const char **argv)
{
    boost::asio::io_context ioctx;
    test::data proto;
    proto.set_str("aaaa");
    proto.mutable_pos2d()->set_pos_x(876);
    proto.mutable_pos2dcf()->set_is_detect(true);

    test::walk_msg walk;
    walk.set_voltage(12.9);
    auto loc = walk.mutable_lotation();
    loc->set_roll(32.1);
    loc->set_pitch(123.1);
    loc->set_yaw(98.0);
    auto a = proto.mutable_walk();
    *a = walk;

    std::cout << proto.DebugString();
    
    iterate_proto_and_post(proto, ioctx);
    std::cout << "      ----- run -----" << std::endl;
    ioctx.run();
    return 0;
}