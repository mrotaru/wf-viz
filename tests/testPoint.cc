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

double check_fraction = 1e-5; // tests fail if difference is greater than 0.00001

BOOST_AUTO_TEST_CASE( Operators )
{
    Point p ( 20,10 );
    Point p2( 10,10 );
    Point p3 = p + p2;
    BOOST_CHECK( p3.getX()==30 && p3.getY()==20 );
}

BOOST_AUTO_TEST_CASE( rotation_around_origin )
{
    Point p1( 100, 100 );
    p1.rotate( 45 );
    BOOST_CHECK_CLOSE_FRACTION( p1.getX(),   0.00,      check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p1.getY(), 141.421356 , check_fraction );
    
    Point p2( 100, 200 );
    p2.rotate( 90 );
    BOOST_CHECK_CLOSE_FRACTION( p2.getX(), -200, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p2.getY(),  100, check_fraction );

    Point p3( 100, 200 );
    p3.rotate( 5 );
    BOOST_CHECK_CLOSE_FRACTION( p3.getX(),  82.18832126,  check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p3.getY(), 207.954513893, check_fraction );

    Point p4( 100, 200 );
    p4.rotate( -5 );
    BOOST_CHECK_CLOSE_FRACTION( p4.getX(), 117.050618359, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p4.getY(), 190.523365344, check_fraction );
}

BOOST_AUTO_TEST_CASE( rotation_around_point )
{
    Point p1( 100, 200 );
    p1.rotate( 45, 200, 100 );
    BOOST_CHECK_CLOSE_FRACTION( p1.getX(),   58.578643763, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p1.getY(),  100.00       , check_fraction );
    
    Point p2( 100, 200 );
    p2.rotate( 90, 200, 100 );
    BOOST_CHECK_CLOSE_FRACTION( p2.getX(), 100.00, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p2.getY(),   0.00, check_fraction ); // FAILS

    Point p3( 100, 200 );
    p3.rotate( 5, 200, 100 );
    BOOST_CHECK_CLOSE_FRACTION( p3.getX(),  91.664955916, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION( p3.getY(), 190.903895534, check_fraction );

    Point p4( 100, 200 );
    p4.rotate( -5, 200, 100 );
    BOOST_CHECK_CLOSE_FRACTION( p4.getX(), 117.050618359, check_fraction ); // FAILS
    BOOST_CHECK_CLOSE_FRACTION( p4.getY(), 109.096104466, check_fraction ); // FAILS
}

BOOST_AUTO_TEST_SUITE_END()
