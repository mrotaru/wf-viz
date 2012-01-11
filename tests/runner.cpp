#include <iostream>
#include "../include/Point.h"

#define BOOST_TEST_MODULE Point test_number_one
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace xmx;

BOOST_AUTO_TEST_CASE( operators_test )
{
    Point p ( 20,10 );
    Point p2( 10,10 );
    Point p3 = p + p2;
    p3.print();
    BOOST_CHECK( p3.getX()==30 && p3.getY()==20 );
}
