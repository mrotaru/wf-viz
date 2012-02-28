#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "../include/Line.h"

using namespace std;
using namespace xmx;

BOOST_AUTO_TEST_SUITE(TestLine)

BOOST_AUTO_TEST_CASE(Test1)
{
    Line l( 10, 10, 20, 20 );
    l.rotate( 45 );
    BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()
