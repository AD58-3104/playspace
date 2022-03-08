#include "data.pb.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>
#include <cstdint>
#include <variant>
#include <boost/hana.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

using proto_tuple = std::tuple<test::data, test::Pos2DCf, test::Pos2D>;
// test::data a;
// test::Pos2DCf b;
// test::Pos2D c;
static const proto_tuple PROTO_LIST(test::data::default_instance(), test::Pos2DCf::default_instance(), test::Pos2D::default_instance());
using proto_variant = std::variant<test::data, test::Pos2DCf, test::Pos2D>;

void createMatchedProto(google::protobuf::FieldDescriptor *field, const test::data message_instance, proto_variant &output)
{
}

template <size_t N = 0>
void iterate_proto_tuple(const proto_tuple &t,const google::protobuf::FieldDescriptor* field)
{
    if constexpr (N < std::tuple_size<proto_tuple>::value)
    {
        const auto &message_instance = std::get<N>(t);
        if (field->message_type() == message_instance.GetDescriptor())
        {
            auto* new_ins = message_instance.New();
            // new_ins = google::protobuf::DynamicCastToGenerated<decltype(new_ins)>(google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New()); //dynamiccastの方が良いかも.
            new_ins->CopyFrom(*(google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New())); //dynamiccastの方が良いかも.
            std::cout << new_ins->DebugString();
        }
        else{
            std::cout << "else | given type::" << field->message_type()->full_name() << " tuple list::" << message_instance.GetDescriptor()->full_name() << std::endl;
        }
        iterate_proto_tuple<N + 1>(t, field);
    }
}

template <class protobuf_type>
void iterate_proto(protobuf_type prt)
{
    const auto rflc = prt.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> listfield;
    rflc->ListFields(prt, &listfield);
    for (const auto &field : listfield)
    {
        // std::cout << field->full_name() << "     " << field->name();
        if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        {
            using namespace test;
            // const auto &aaa =  rflc->MutableMessage(&prt,field,rflc->GetMessageFactory());
            auto func = [&field](const auto &message_instance)
            {
                if (field->message_type()->name() == message_instance.GetDescriptor()->name())
                {
                    auto new_ins = message_instance.New();
                    new_ins = google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New();
                }
            };
            iterate_proto_tuple<0>(PROTO_LIST,field);
            // std::cout << " message " << field->message_type()->name();
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
    test::data proto;
    proto.set_str("aaaa");
    proto.set_x_vec(765);
    proto.set_y_vec(346);
    proto.set_z_vec(315);
    proto.set_cnt_i(283);
    proto.set_cnt_j(876);
    proto.mutable_pos2d()->set_pos_x(876);
    proto.mutable_pos2dcf()->set_is_detect(true);
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
    iterate_proto<test::data>(proto);

    return 0;
}
