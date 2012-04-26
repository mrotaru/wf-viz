#ifndef DATA_UTILS_CPP
#define DATA_UTILS_CPP

#include "data_utils.h"

#include <iostream>
#include <string>
using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

namespace xml {

//------------------------------------------------------------------------------
void loadXMLData( string file_name )
{
    using boost::property_tree::ptree;
    ptree pt;

    // reading file.xml
    read_xml( file_name, pt );

    int couted = 0;
    BOOST_FOREACH( ptree::value_type const& v, pt.get_child( "Root" ).get_child( "data" ) )
    {
        if( v.second.get< string >( "field.<xmlattr>.key", "-" ) == "ROU" )
        {
            BOOST_FOREACH( ptree::value_type const& v2, v.second )
            {
                string name = v2.second.get< string >( "<xmlattr>.name", "--" );
                if( name == "Year" )
                {
                    cout << v2.second.get< int >( "" );
                }
                else if( name == "Value" )
                {
                    cout << " " << v2.second.get< double >("", -1000.0f ) << endl;
                }
            }

        couted++;
        }
    }
}

} // namespace xmx
#endif /* DATA_UTILS_CPP */
