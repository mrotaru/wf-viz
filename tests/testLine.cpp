#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include "../include/Line.h"

using namespace std;
using namespace xmx;

struct Fixture
{
    Fixture() 
    { 
        l  = boost::shared_ptr<Line>( new Line( 100, 100, 300, 300 ) ); 
        l2 = boost::shared_ptr<Line>( new Line( 100, 100, 100, 100 ) ); 
    }
    ~Fixture() {}
    boost::shared_ptr<Line> l;
    boost::shared_ptr<Line> l2;
};

BOOST_FIXTURE_TEST_SUITE( TestLine, Fixture )

double check_fraction = 1e-5; // tests fail if difference is greater than 0.00001

BOOST_AUTO_TEST_CASE( TestDistance )
{
    BOOST_CHECK_CLOSE_FRACTION(  l->distanceTo(   0.0,   0.0 ), 141.42135,   check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->distanceTo( 200.0, 200.0 ),   0.0,       check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->distanceTo( 100.0, 200.0 ),  70.7106781, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->distanceTo( 100.0,-100.0 ), 200.0, check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->distanceTo( 400.0, 300.0 ), 100.0, check_fraction );
}

BOOST_AUTO_TEST_CASE( Rotation )
{
    l->rotate( 45 );
    BOOST_CHECK_CLOSE_FRACTION(  l->getX1(), 200.0,     check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->getY1(),  58.5786,  check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->getX2(), 200.0,     check_fraction );
    BOOST_CHECK_CLOSE_FRACTION(  l->getY2(), 341.4214,  check_fraction );
}

BOOST_AUTO_TEST_SUITE_END()
