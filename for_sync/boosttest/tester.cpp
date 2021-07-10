#define BOOST_TEST_MODULE test_example
#include <boost/test/unit_test.hpp>
#include "func.hpp"

BOOST_AUTO_TEST_CASE( test1 )
{
    const double a = -1.23456e-3;
    const double b = 3.2;
    BOOST_CHECK_EQUAL( f2(a,b), a*b );
}

BOOST_AUTO_TEST_CASE( test2 )
{
    const int a = 3;
    const int b = 4;
    BOOST_CHECK_EQUAL( f1(a,b), a + b );
    BOOST_CHECK_EQUAL( f1(-2,3), -1 );
    BOOST_CHECK_EQUAL( f1(2,-3), -1 );
    BOOST_CHECK_EQUAL( f1(-2,-3), -1 );
}