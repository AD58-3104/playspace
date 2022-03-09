#include <iostream>

struct Base
{
    virtual void func()
    {
        std::cout << "Base::func()\n";
    }
};
 
struct Derived : public Base
{
    void func()
    {
        std::cout << "Derived::func()\n";
    }
};
 
void foo(Base* iBase)
{
    iBase->func();
}
int main()
{
    Base aBase;
    Derived aDerived;
    int i = 0;
    std::cin >> i;
    if (i > 10)
        foo(&aBase);
    else
        foo(&aDerived);
}