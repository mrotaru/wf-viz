#include <iostream>
#include <limits>
using namespace std;

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "xml_data.h"

namespace xmx
{

// Will create a map of shared pointers to maps with data from `file_name` of the form:
//---------------------------------------------------------------------------------------
//      // ISO3 country code         year      value  - -  - > IMPORTANT!                  
//  {         |                       |          |             -------------------------------
//            |     shared_ptr        |          |             if no data is available        
//            |   < map<int,double> > |          |             for a particular year, then
//            V          V            V          V             the value stored will be     
//        [ "ROU" ] =>   *  -> {    [ 1990 ] =>    -1          -1                             
//                                  [ 1991 ] => 240000000            
//                                    ...                        
//                             }                                 
//                                                               
//        [ "MDA" ] =>   *  -> {    [ 1990 ] => 13000000
//                                  [ 1991 ] => 14000000
//                                    ...
//                             }
//           ...
//  }
//---------------------------------------------------------------------------------------
void XMLData::load( string file_name )
{
    using boost::property_tree::ptree;
    ptree pt;

    cout << "loading xml data from: " << file_name << endl;
    read_xml( file_name, pt );

    auto ignored = set< string>();
    int couted = 0;
    BOOST_FOREACH( ptree::value_type const& v, pt.get_child( "Root" ).get_child( "data" ) )
    {
        string country_code = v.second.get< string >( "field.<xmlattr>.key", "-" );
        if( ISO3_codes.find( country_code ) != ISO3_codes.end() )
        {
            int year = -1;
            double value = -1;
            BOOST_FOREACH( ptree::value_type const& v2, v.second )
            {
                string name = v2.second.get< string >( "<xmlattr>.name", "--" );

                if( name == "Year" )
                     year = v2.second.get< int >( "" );

                else if( name == "Value" )
                     value = v2.second.get< double >("", -1.0f );

            }
            // cout << "year: " << year << ", value: " << value << endl;

            if( data.find( country_code ) == data.end() )
                data[ country_code ] = shared_ptr< map< int, double > >( new map< int, double > );

            data[ country_code ]->insert( pair< int, double>( year, value ) );
           
        couted++;
        }
        else
        {
            if( ignored.find( country_code ) == ignored.end() )
            {
                cout << "ignoring: " << country_code << endl;
                ignored.insert( country_code );
            }
        }
    }
    loaded = true;
}

// The following functions are used to interrogate the `data` map.
//------------------------------------------------------------------------------
double XMLData::getMaxDataValue( int year )
{
    if( !loaded ) return -1;

    double max = numeric_limits< double >::min();
    string max_country = "none";
    
    for( auto it = data.begin(); it != data.end(); it++ )
    {
        map< int, double > map_ = *(it)->second;
        double value = map_[ year ];

        // see if we have a value for the requested year;
        // remember, -1 is used to signify 'N/A' - not available
        //----------------------------------------------------------------------
        if( value != -1 && value > max )
        {
            max = value;
            max_country = it->first;
        }
    }
    return max;
}

//------------------------------------------------------------------------------
double XMLData::getMinDataValue( int year )
{
    if( !loaded ) return -1;

    double min = numeric_limits< double >::max();
    string min_country = "none";
    
    for( auto it = data.begin(); it != data.end(); it++ )
    {
        map< int, double > map_ = *(it)->second;
        double value = map_[ year ];

        // see if we have a value for the requested year;
        // remember, -1 is used to signify 'N/A' - not available
        //----------------------------------------------------------------------
        if( value != -1 && value < min )
        {
            min = value;
            min_country = it->first;
        }
    }
    return min;
}

// Get the minimum year for which there is data
//------------------------------------------------------------------------------
int XMLData::getMaxDataYear()
{
    if( !loaded ) return -1;

    int max =  numeric_limits< int >::min();
    string max_country = "none";
    for( auto it = data.begin(); it != data.end(); it++ )
    {
        auto map_ = *(it)->second;
        for( auto i_map_ = map_.begin(); i_map_ != map_.end(); i_map_++ )
        {
            int year = i_map_->first;
            double value = i_map_->second;
            if( value != -1 &&  year > max )
            {
                max = year;
                max_country = it->first;
            }
        }
    }
    return max;
}

// Get the maximum year for which there is data
//------------------------------------------------------------------------------
int XMLData::getMinDataYear()
{
    if( !loaded ) return -1;

    int min = numeric_limits< int >::max();
    string min_country = "none";
    for( auto it = data.begin(); it != data.end(); it++ )
    {
        auto map_ = *(it)->second;
        for( auto i_map_ = map_.begin(); i_map_ != map_.end(); i_map_++ )
        {
            int year = i_map_->first;
            double value = i_map_->second;
            if( value != -1 && year < min )
            {
                min = year;
                min_country = it->first;
            }
        }
    }
    return min;
}

// Returns the value for `iso3_code` for year `year`
// Ex: getValueForYear( "ROU", 1987 ) will return '38067565160.5839'
//------------------------------------------------------------------------------
double XMLData::getValueForYear( string iso3_code, int year )
{
    if( !loaded ) return -1;

    map< int, double > map_ = *( data[ iso3_code ] );
    return map_[ year ];
}

map< string, string > ISO3_codes = map_list_of
        ( "ATG", "Antigua and Barbuda" )
        ( "DZA", "Algeria" )
        ( "AZE", "Azerbaijan" )
        ( "ALB", "Albania" )
        ( "ARM", "Armenia" )
        ( "AGO", "Angola" )
        ( "ASM", "American Samoa" )
        ( "ARG", "Argentina" )
        ( "AUS", "Australia" )
        ( "BHR", "Bahrain" )
        ( "BRB", "Barbados" )
        ( "BMU", "Bermuda" )
        ( "BHS", "Bahamas" )
        ( "BGD", "Bangladesh" )
        ( "BLZ", "Belize" )
        ( "BIH", "Bosnia and Herzegovina" )
        ( "BOL", "Bolivia" )
        ( "MMR", "Burma" )
        ( "BEN", "Benin" )
        ( "SLB", "Solomon Islands" )
        ( "BRA", "Brazil" )
        ( "BGR", "Bulgaria" )
        ( "BRN", "Brunei Darussalam" )
        ( "CAN", "Canada" )
        ( "KHM", "Cambodia" )
        ( "LKA", "Sri Lanka" )
        ( "COG", "Congo" )
        ( "COD", "Democratic Republic of the Congo" )
        ( "BDI", "Burundi" )
        ( "CHN", "China" )
        ( "AFG", "Afghanistan" )
        ( "BTN", "Bhutan" )
        ( "CHL", "Chile" )
        ( "CYM", "Cayman Islands" )
        ( "CMR", "Cameroon" )
        ( "TCD", "Chad" )
        ( "COM", "Comoros" )
        ( "COL", "Colombia" )
        ( "CRI", "Costa Rica" )
        ( "CAF", "Central African Republic" )
        ( "CUB", "Cuba" )
        ( "CPV", "Cape Verde" )
        ( "COK", "Cook Islands" )
        ( "CYP", "Cyprus" )
        ( "DNK", "Denmark" )
        ( "DJI", "Djibouti" )
        ( "DMA", "Dominica" )
        ( "DOM", "Dominican Republic" )
        ( "ECU", "Ecuador" )
        ( "EGY", "Egypt" )
        ( "IRL", "Ireland" )
        ( "GNQ", "Equatorial Guinea" )
        ( "EST", "Estonia" )
        ( "ERI", "Eritrea" )
        ( "SLV", "El Salvador" )
        ( "ETH", "Ethiopia" )
        ( "AUT", "Austria" )
        ( "CZE", "Czech Republic" )
        ( "GUF", "French Guiana" )
        ( "FIN", "Finland" )
        ( "FJI", "Fiji" )
        ( "FLK", "Falkland Islands (Malvinas)" )
        ( "FSM", "Micronesia, Federated States of" )
        ( "PYF", "French Polynesia" )
        ( "FRA", "France" )
        ( "GMB", "Gambia" )
        ( "GAB", "Gabon" )
        ( "GEO", "Georgia" )
        ( "GHA", "Ghana" )
        ( "GRD", "Grenada" )
        ( "GRL", "Greenland" )
        ( "DEU", "Germany" )
        ( "GUM", "Guam" )
        ( "GRC", "Greece" )
        ( "GTM", "Guatemala" )
        ( "GIN", "Guinea" )
        ( "GUY", "Guyana" )
        ( "HTI", "Haiti" )
        ( "HND", "Honduras" )
        ( "HRV", "Croatia" )
        ( "HUN", "Hungary" )
        ( "ISL", "Iceland" )
        ( "IND", "India" )
        ( "IRN", "Iran (Islamic Republic of)" )
        ( "ISR", "Israel" )
        ( "ITA", "Italy" )
        ( "CIV", "Cote d'Ivoire" )
        ( "IRQ", "Iraq" )
        ( "JPN", "Japan" )
        ( "JAM", "Jamaica" )
        ( "JOR", "Jordan" )
        ( "KEN", "Kenya" )
        ( "KGZ", "Kyrgyzstan" )
        ( "PRK", "Korea, Democratic People's Republic of" )
        ( "KIR", "Kiribati" )
        ( "KOR", "Korea, Republic of" )
        ( "KWT", "Kuwait" )
        ( "KAZ", "Kazakhstan" )
        ( "LAO", "Lao People's Democratic Republic" )
        ( "LBN", "Lebanon" )
        ( "LVA", "Latvia" )
        ( "BLR", "Belarus" )
        ( "LTU", "Lithuania" )
        ( "LBR", "Liberia" )
        ( "SVK", "Slovakia" )
        ( "LIE", "Liechtenstein" )
        ( "LBY", "Libyan Arab Jamahiriya" )
        ( "MDG", "Madagascar" )
        ( "MTQ", "Martinique" )
        ( "MNG", "Mongolia" )
        ( "MSR", "Montserrat" )
        ( "MKD", "The former Yugoslav Republic of Macedonia" )
        ( "MLI", "Mali" )
        ( "MAR", "Morocco" )
        ( "MUS", "Mauritius" )
        ( "MRT", "Mauritania" )
        ( "MLT", "Malta" )
        ( "OMN", "Oman" )
        ( "MDV", "Maldives" )
        ( "MEX", "Mexico" )
        ( "MYS", "Malaysia" )
        ( "MOZ", "Mozambique" )
        ( "MWI", "Malawi" )
        ( "NCL", "New Caledonia" )
        ( "NIU", "Niue" )
        ( "NER", "Niger" )
        ( "ABW", "Aruba" )
        ( "AIA", "Anguilla" )
        ( "BEL", "Belgium" )
        ( "HKG", "Hong Kong" )
        ( "MNP", "Northern Mariana Islands" )
        ( "FRO", "Faroe Islands" )
        ( "AND", "Andorra" )
        ( "GIB", "Gibraltar" )
        ( "IMN", "Isle of Man" )
        ( "LUX", "Luxembourg" )
        ( "MAC", "Macau" )
        ( "MCO", "Monaco" )
        ( "PSE", "Palestine" )
        ( "MNE", "Montenegro" )
        ( "MYT", "Mayotte" )
        ( "ALA", "Åland Islands" )
        ( "NFK", "Norfolk Island" )
        ( "CCK", "Cocos (Keeling) Islands" )
        ( "ATA", "Antarctica" )
        ( "BVT", "Bouvet Island" )
        ( "ATF", "French Southern and Antarctic Lands" )
        ( "HMD", "Heard Island and McDonald Islands" )
        ( "IOT", "British Indian Ocean Territory" )
        ( "CXR", "Christmas Island" )
        ( "UMI", "United States Minor Outlying Islands" )
        ( "VUT", "Vanuatu" )
        ( "NGA", "Nigeria" )
        ( "NLD", "Netherlands" )
        ( "NOR", "Norway" )
        ( "NPL", "Nepal" )
        ( "NRU", "Nauru" )
        ( "SUR", "Suriname" )
        ( "NIC", "Nicaragua" )
        ( "NZL", "New Zealand" )
        ( "PRY", "Paraguay" )
        ( "PER", "Peru" )
        ( "PAK", "Pakistan" )
        ( "POL", "Poland" )
        ( "PAN", "Panama" )
        ( "PRT", "Portugal" )
        ( "PNG", "Papua New Guinea" )
        ( "GNB", "Guinea-Bissau" )
        ( "QAT", "Qatar" )
        ( "REU", "Reunion" )
        ( "ROU", "Romania" )
        ( "MDA", "Republic of Moldova" )
        ( "PHL", "Philippines" )
        ( "PRI", "Puerto Rico" )
        ( "RUS", "Russia" )
        ( "RWA", "Rwanda" )
        ( "SAU", "Saudi Arabia" )
        ( "KNA", "Saint Kitts and Nevis" )
        ( "SYC", "Seychelles" )
        ( "ZAF", "South Africa" )
        ( "LSO", "Lesotho" )
        ( "BWA", "Botswana" )
        ( "SEN", "Senegal" )
        ( "SVN", "Slovenia" )
        ( "SLE", "Sierra Leone" )
        ( "SGP", "Singapore" )
        ( "SOM", "Somalia" )
        ( "ESP", "Spain" )
        ( "LCA", "Saint Lucia" )
        ( "SDN", "Sudan" )
        ( "SWE", "Sweden" )
        ( "SYR", "Syrian Arab Republic" )
        ( "CHE", "Switzerland" )
        ( "TTO", "Trinidad and Tobago" )
        ( "THA", "Thailand" )
        ( "TJK", "Tajikistan" )
        ( "TKL", "Tokelau" )
        ( "TON", "Tonga" )
        ( "TGO", "Togo" )
        ( "STP", "Sao Tome and Principe" )
        ( "TUN", "Tunisia" )
        ( "TUR", "Turkey" )
        ( "TUV", "Tuvalu" )
        ( "TKM", "Turkmenistan" )
        ( "TZA", "United Republic of Tanzania" )
        ( "UGA", "Uganda" )
        ( "GBR", "United Kingdom" )
        ( "UKR", "Ukraine" )
        ( "USA", "United States" )
        ( "BFA", "Burkina Faso" )
        ( "URY", "Uruguay" )
        ( "UZB", "Uzbekistan" )
        ( "VCT", "Saint Vincent and the Grenadines" )
        ( "VEN", "Venezuela" )
        ( "VGB", "British Virgin Islands" )
        ( "VNM", "Viet Nam" )
        ( "VIR", "United States Virgin Islands" )
        ( "NAM", "Namibia" )
        ( "WLF", "Wallis and Futuna Islands" )
        ( "WSM", "Samoa" )
        ( "SWZ", "Swaziland" )
        ( "YEM", "Yemen" )
        ( "ZMB", "Zambia" )
        ( "ZWE", "Zimbabwe" )
        ( "IDN", "Indonesia" )
        ( "GLP", "Guadeloupe" )
        ( "ANT", "Netherlands Antilles" )
        ( "ARE", "United Arab Emirates" )
        ( "TLS", "Timor-Leste" )
        ( "PCN", "Pitcairn Islands" )
        ( "PLW", "Palau" )
        ( "MHL", "Marshall Islands" )
        ( "SPM", "Saint Pierre and Miquelon" )
        ( "SHN", "Saint Helena" )
        ( "SMR", "San Marino" )
        ( "TCA", "Turks and Caicos Islands" )
        ( "ESH", "Western Sahara" )
        ( "SRB", "Serbia" )
        ( "VAT", "Holy See (Vatican City)" )
        ( "SJM", "Svalbard" )
        ( "MAF", "Saint Martin" )
        ( "BLM", "Saint Barthelemy" )
        ( "GGY", "Guernsey" )
        ( "JEY", "Jersey" )
        ( "SGS", "South Georgia South Sandwich Islands" )
        ( "TWN", "Taiwan" );



} // namespace xmx
