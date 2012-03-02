#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "../include/Point.h"

using namespace std;
using namespace xmx;

BOOST_AUTO_TEST_SUITE( TestPoint )

BOOST_AUTO_TEST_CASE( Operators )
{
    Point p ( 20,10 );
    Point p2( 10,10 );
    Point p3 = p + p2;
    BOOST_CHECK( p3.getX()==30 && p3.getY()==20 );
}

BOOST_AUTO_TEST_CASE( Rotation )
{
    Point p( 100, 100 );
    p.rotate( 45 );
    p.print();
    
    Point p2( 100, 100 );
    p2.rotate( 90 );
    p2.print();
}

BOOST_AUTO_TEST_SUITE_END()
