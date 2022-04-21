#include <iostream>
#include <string>
#include "data.pb.h"

int main(int argc, char const *argv[])
{
    test::caseone a;
    test::casetwo b;
    for (size_t i = 0; i < 100; i++)
    {
        a.add_a(i);
        b.add_a(i);
        if (i == 54)
        {
            a.add_a(-1);
            b.add_a(7650);
        }
    }
    std::cout << "the size of sint32 is " << a.SerializeAsString().length() << std::endl
              << "the size of int32 is " << b.SerializeAsString().length() << std::endl;
    return 0;
}

//不利なやつ無し
/*
sint32 139
int32 102

1つだけ
140
104

6こ
145
114

int32に1つだけ-1 114
*/