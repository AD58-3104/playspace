#include "data.pb.h"
#include <iostream>
#include <vector>
#include <utility>
#include <cstdint>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>


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
    // proto.mutable_pos2dcf()->set_is_detect(true);
    const auto rflc = proto.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor*> listfield;
    rflc->ListFields(proto,&listfield);
    for(const auto &field:listfield){
        std::cout << field->full_name() << "     " << field->name();
        if(field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE){
            const auto &aaa =  google::protobuf::MessageFactory::generated_factory()->GetPrototype(field->message_type())->New();
            // const auto &aaa =  rflc->MutableMessage(&proto,field,rflc->GetMessageFactory());
            std::cout << "message "  << aaa->x_vec();
        }
        else{ //fieldの場合
            if(field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE){
                std::cout << " value::" << rflc->GetDouble(proto,field);
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
