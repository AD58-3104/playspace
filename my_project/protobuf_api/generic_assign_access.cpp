#include "data.pb.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>
#include <cstdint>
#include <variant>
#include <boost/asio.hpp>
#include <boost/pfr/core.hpp>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

struct Pos2D
{
    double pos_x = 0;
    double pos_y = 0;
    double pos_theta = 0;
};

// constexpr auto getGenericField

template <typename set_target_t>
void getField(const google::protobuf::FieldDescriptor::Type &type_name,
              const google::protobuf::Message &msg,
              const google::protobuf::FieldDescriptor *field,
              set_target_t &set_target_field)
{
    {
        if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_FLOAT == type_name)
        {
            set_target_field = (msg.GetReflection()->GetFloat(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_DOUBLE == type_name)
        {
            set_target_field = (msg.GetReflection()->GetDouble(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_INT32 == type_name)
        {
            set_target_field = (msg.GetReflection()->GetInt32(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_INT64 == type_name)
        {
            set_target_field = (msg.GetReflection()->GetInt64(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_UINT32 == type_name)
        {
            set_target_field = (msg.GetReflection()->GetUInt32(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_UINT64 == type_name)
        {
            set_target_field = (msg.GetReflection()->GetUInt64(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_BOOL == type_name)
        {
            set_target_field = (msg.GetReflection()->GetBool(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_STRING == type_name)
        {
            set_target_field = (msg.GetReflection()->GetString(msg, field));
        }
        else if constexpr (google::protobuf::FieldDescriptor::Type::TYPE_ENUM == type_name)
        {
            set_target_field = (msg.GetReflection()->GetEnumValue(msg, field));
        }
        // case google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE:
        //     return (msg.GetReflection()->GetMessage(msg, field));
        //     break;
    }
}

template <typename protobuf_msg_t, typename user_defined_t, size_t field_number = 0>
void genericAccess(const protobuf_msg_t &proto_data, user_defined_t &user_defined_data)
{
    if constexpr (field_number < boost::pfr::tuple_size<user_defined_t>::value)
    {
        const auto descriptor = proto_data.GetDescriptor();
        auto field = descriptor->field(field_number);
        auto &set_target = boost::pfr::get<field_number>(user_defined_data);
        getField(field->type(), proto_data, field, set_target);
        genericAccess<protobuf_msg_t, user_defined_t, field_number + 1>(proto_data, user_defined_data);
    }
}

int main(int argc, char const *argv[])
{

    Pos2D pos{};
    test::Pos2D data;
    data.set_pos_x(1);
    data.set_pos_y(5);
    data.set_pos_theta(19);
    genericAccess(data, pos);
    auto &x = boost::pfr::get<0>(pos);
    auto &y = boost::pfr::get<1>(pos);
    auto &z = boost::pfr::get<2>(pos);
    std::cout << x << y << z << std::endl;
    return 0;
}
