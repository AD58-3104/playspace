#include "data.pb.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>
#include <cstdint>
#include <variant>
#include <boost/asio.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

#define PROTOBUF_CLASS_LIST test::data, test::Pos2DCf, test::Pos2D , test::walk_msg
using proto_tuple = std::tuple<PROTOBUF_CLASS_LIST>;
using proto_variant = std::variant<PROTOBUF_CLASS_LIST>;
#undef PROTOBUF_CLASS_LIST
// static const proto_tuple PROTO_LIST(test::data::default_instance(), test::Pos2DCf::default_instance(), test::Pos2D::default_instance(),test::walk_msg::default_instance());
static const  proto_tuple PROTO_LIST = {};
template <typename... Ts>
struct protobufTypeList
{
    using proto_tuple = std::tuple<Ts...>;
    using proto_variant = std::variant<Ts...>;
    static proto_tuple get(){

    };
    static inline constexpr size_t tuple_size = sizeof...(Ts);
    static inline const proto_tuple List ;
    
};

static const protobufTypeList<test::data, test::Pos2DCf, test::Pos2D , test::walk_msg> PROTO_MSG_LIST;

/**
 * @brief オーバーロード解決が面倒なのでこの中でポストした方が良いかも?
 * でも寿命が相変わらず怪しい.コピーだから平気かな.コピーが多くなってるからここの一回だけでなんとかしたいね.
 *
 */
struct ProcessWork
{
    ProcessWork(boost::asio::io_context &io_ctx) : io_ctx_(io_ctx) {}

    //テンプレート
    // void operator()(対象のメッセージ型)
    // {
        // io_ctx_.post(
        // [data = data]() {
        //-----------------------------------------------------

    //ここに処理を書く

        //-----------------------------------------------------
        // });
    // }


    void operator()(test::data&&  data)
    {
        io_ctx_.post(
            [data = data]()
            { 
                // std::cout << data.DebugString();
            });
    }
    void operator()(test::Pos2D&&  data)
    {
        io_ctx_.post(
            [data = data]()
            {
                if (data.has_pos_x())
                    std::cout << "Pos2D  " << data.pos_x() << std::endl;
            });
    }
    void operator()(test::Pos2DCf&&  data)
    {
        io_ctx_.post(
            [data = data]()
            {
                if (data.has_is_detect())
                {
                    std::cout << "Pos2DCf  " << data.is_detect() << std::endl;
                }
            });
    }
    void operator()(test::walk_msg&& data){
        io_ctx_.post([data=data](){
            // std::cout << data.DebugString();
        });
    }

private:
    boost::asio::io_context &io_ctx_;
};

void createMatchedProto(google::protobuf::FieldDescriptor *field, const test::data message_instance, proto_variant &output)
{
}

template <size_t N = 0>
void iterate_proto_tuple(const proto_tuple &t, const google::protobuf::FieldDescriptor *field, google::protobuf::Message &msg_instance, boost::asio::io_context &io_ctx)
{
    if constexpr (N < std::tuple_size<proto_tuple>::value)
    {
        auto type_ref = std::get<N>(t);
        const auto &reference_instance = decltype(type_ref)::default_instance();
        if (field->message_type() == reference_instance.GetDescriptor())
        {
            auto *new_ins_ptr = reference_instance.New();
            // new_ins_ptr = google::protobuf::DynamicCastToGenerated<decltype(new_ins_ptr)>(google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New()); //無理.
            // new_ins_ptr->CopyFrom(*(google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New())); //可能なやつ.
            const auto rflc = msg_instance.GetReflection();
            new_ins_ptr->CopyFrom(rflc->GetMessage(msg_instance, field));
            proto_variant new_instance(*new_ins_ptr);
            // io_ctx.io_ctx_.post(std::bind(std::visit, io_ctx, new_instance)); //寿命がかなり怪しい
            std::visit(ProcessWork{io_ctx}, std::move(new_instance));
            std::cout << "match! " <<  field->message_type()->full_name() << std::endl;
        }
        else
        {
            // std::cout << "else | given ::" << field->message_type()->full_name() << " tuple ::" << reference_instance.GetDescriptor()->full_name() << std::endl;
        }
        iterate_proto_tuple<N + 1>(t, field, msg_instance, io_ctx);
    }
}

template <class protobuf_type>
void iterate_proto_and_post(protobuf_type prt, boost::asio::io_context &io_ctx)
{
    const auto rflc = prt.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> listfield;
    rflc->ListFields(prt, &listfield);
    for (const auto &field : listfield)
    {
        // std::cout << field->full_name() << "     " << field->name();
        if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        {
            // using namespace test;
            // const auto &aaa =  rflc->MutableMessage(&prt,field,rflc->GetMessageFactory());
            // auto func = [&field](const auto &message_instance)
            // {
            //     if (field->message_type()->name() == message_instance.GetDescriptor()->name())
            //     {
            //         auto new_ins = message_instance.New();
            //         new_ins = google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New();
            //     }
            // };
            iterate_proto_tuple<0>(PROTO_MSG_LIST.List, field, prt, io_ctx);
        }
        else
        { // fieldの場合
            if (field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE)
            {
                // std::cout << " value::" << rflc->GetDouble(prt, field);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    boost::asio::io_context ioctx;
    // ProcessWork worker(ioctx);
    test::data proto;
    proto.set_str("aaaa");
    proto.set_x_vec(765);
    proto.set_y_vec(346);
    proto.set_z_vec(315);
    proto.set_cnt_i(283);
    proto.set_cnt_j(876);
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
    // std::cout << proto.DebugString();
    iterate_proto_and_post(proto, ioctx);
    std::cout << "----- run -----" << std::endl;
    ioctx.run();
    return 0;
}

// const auto rflc = proto.GetReflection();
// std::vector<const google::protobuf::FieldDescriptor *> listfield;
// rflc->ListFields(proto, &listfield);
// for (const auto &field : listfield)
// {
//     std::cout << field->full_name() << "     " << field->name();
//     if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
//     {
//         // const auto &aaa =  google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New();
//         // const auto &aaa =  rflc->MutableMessage(&proto,field,rflc->GetMessageFactory());
//         std::cout << " message " << field->message_type()->name();
//     }
//     else
//     { // fieldの場合
//         if (field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE)
//         {
//             std::cout << " value::" << rflc->GetDouble(proto, field);
//         }
//     }
//     std::cout << std::endl;
// }