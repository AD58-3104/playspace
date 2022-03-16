#include <iostream>
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>
#include <cstdint>
#include <variant>

struct a
{
    int i = 0;
    void f()
    {
        if (i > 20)
        {
            return;
        }
        std::cout << i << std::endl;
        i++;
        f();
    }
    void fu(){
        std::cout << "a\n";
    }
};
struct b{
    void fu(){
        std::cout << "b\n";
    }
};

struct c
{
    void fu(){
        std::cout << "c\n";
    }
};


template <typename... Ts>
struct typeList
{
    using proto_tuple = std::tuple<Ts...>;
    using proto_variant = std::variant<Ts...>;
    static inline constexpr size_t tuple_size = sizeof...(Ts);
    static inline constexpr proto_tuple PROTO_LIST = {};
    constexpr void f(){
        // for(size_t i = 0; i < tuple_size;++i){
        //     auto ins = std::get<i>(PROTO_LIST);
        //     ins.fu();
        // }
    }
};

int main(int argc, char const *argv[])
{
    a ins;
    std::tuple<a,int,double> tup;
    typeList<a,b,c> tlist;
    tlist.f();
    std::tuple<int> v;
    auto x = std::get<0>(v);
    std::cout << x;
    return 0;
}
