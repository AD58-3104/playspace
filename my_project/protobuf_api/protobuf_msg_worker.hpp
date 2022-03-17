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

template <typename... Ts>
struct protobufTypeList
{
    using proto_tuple_t = std::tuple<Ts...>;
    using proto_variant_t = std::variant<Ts...>;
    static proto_tuple_t get(){

    };
    static inline constexpr size_t tuple_size = sizeof...(Ts);
    static inline const proto_tuple_t List ;
};


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

    //ここにdataに対する処理を書く

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

static const protobufTypeList<test::data, test::Pos2DCf, test::Pos2D , test::walk_msg> PROTO_MSG_LIST;



template <typename scoped_proto_tuple_t,typename scoped_proto_variant_t,size_t N = 0>
void iterate_proto_tuple(const scoped_proto_tuple_t &t, const google::protobuf::FieldDescriptor *field,const google::protobuf::Message &posting_msg_instance, boost::asio::io_context &io_ctx)
{
    if constexpr (N < std::tuple_size<scoped_proto_tuple_t>::value)
    {
        auto type_ref = std::get<N>(t);
        const auto &reference_instance = decltype(type_ref)::default_instance(); 
        if (field->message_type() == reference_instance.GetDescriptor())
        {
            auto *new_ins_ptr = reference_instance.New();//protobufのfactoryを使っていないので多分ちゃんとしたコピーが出来ている筈...。所有権も呼び出し元が持てる
            const auto rflc = posting_msg_instance.GetReflection();
            new_ins_ptr->CopyFrom(rflc->GetMessage(posting_msg_instance, field));
            scoped_proto_variant_t new_instance(*new_ins_ptr);
            std::visit(ProcessWork{io_ctx}, std::move(new_instance));
            std::cout << "match! " <<  field->message_type()->full_name() << std::endl;
        }
        else
        {
        }
        iterate_proto_tuple<scoped_proto_tuple_t,scoped_proto_variant_t   ,N + 1>(t, field, posting_msg_instance, io_ctx);
    }
}

template <class protobuf_type>
void iterate_proto_and_post(protobuf_type posting_msg, boost::asio::io_context &io_ctx)
{
    const auto rflc = posting_msg.GetReflection();
    std::vector<const google::protobuf::FieldDescriptor *> fieldlist;
    rflc->ListFields(posting_msg, &fieldlist);
    for (const auto &field : fieldlist)
    {
        if (field->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
        {
            iterate_proto_tuple<0>(PROTO_MSG_LIST.List, field, posting_msg, io_ctx);
        }
        else
        { 
            // fieldの場合
            if (field->type() == google::protobuf::FieldDescriptor::TYPE_DOUBLE)
            {
            }
        }
    }
}
