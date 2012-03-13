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
    Fixture() { l = boost::shared_ptr<Line>(new Line( 100, 100, 200, 100 )); }
    ~Fixture() {}
    boost::shared_ptr<Line> l;
};

BOOST_FIXTURE_TEST_SUITE( TestLine, Fixture )

BOOST_AUTO_TEST_CASE( TestDistance )
{
    BOOST_CHECK( l->distanceTo( 150,200 ) == 100.0 );
}

BOOST_AUTO_TEST_CASE(Test1)
{
//    l->print("before rotation: ");
    l->rotate( 45 );
//    l->print("after rotation:  ");
    BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()
