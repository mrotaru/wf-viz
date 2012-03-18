#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
using namespace std;

#include "../include/utils.h"
using namespace xmx;

BOOST_AUTO_TEST_SUITE( TestUtils )

double check_fraction = 1e-5; // tests fail if difference is greater than 0.00001
double warn_fraction  = 1e-10; // warn if difference is greater than 10^-10

BOOST_AUTO_TEST_CASE( convert_degrees_to_radians )
{
    BOOST_CHECK_CLOSE_FRACTION( degrees_to_radians( 45 ), 0.785398163  , check_fraction );
    BOOST_WARN_CLOSE_FRACTION ( degrees_to_radians( 45 ), 0.785398163  , warn_fraction  );
    BOOST_CHECK_CLOSE_FRACTION( degrees_to_radians( 1  ), 0.0174532925 , check_fraction );
    BOOST_WARN_CLOSE_FRACTION ( degrees_to_radians( 1  ), 0.0174532925 , warn_fraction  );
    BOOST_CHECK_CLOSE_FRACTION( degrees_to_radians( 90 ), 1.57079633   , check_fraction );
    BOOST_WARN_CLOSE_FRACTION ( degrees_to_radians( 90 ), 1.57079633   , warn_fraction  );
}

BOOST_AUTO_TEST_CASE( distance_between_two_points )
{
    BOOST_CHECK_CLOSE_FRACTION( dist( 100, 100, 300, 300 ), 282.842712, check_fraction );
}

BOOST_AUTO_TEST_SUITE_END()
