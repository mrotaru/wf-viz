#ifndef UTILS_CPP
#define UTILS_CPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <map>
using namespace std;

#ifdef _MSC_VER
#include <windows.h>
#endif

#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/glu.h>
#include <GL/glut.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/assign.hpp"
using namespace boost::assign;

#include "globals.h"
#include "utils.h"

namespace xmx {

// if `str` matches `myRegex`, try parsing the first match into an integer
// NOTE: lexical_cast is said to be slow 
// [http://stackoverflow.com/questions/1250795/very-poor-boostlexical-cast-performance]
//-----------------------------------------------------------------------------
template < typename T >
T getFromRegex( string haystack, const boost::regex& myRegex )
{
    boost::smatch match;
    if( regex_search( haystack, match, myRegex ))
        return boost::lexical_cast<T>( match[1] );
    else
        throw( "exception in getFromRegex( string, regex )");
}

template int getFromRegex< int >( string haystack, const boost::regex& myRegex );
template string getFromRegex< string >( string haystack, const boost::regex& myRegex );

// return a string representation of `p`
// from: http://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/#comment-22
//-----------------------------------------------------------------------------
template <class T>
inline std::string to_string( const T& p )
{
    std::stringstream ss;
    ss << p;
    return ss.str();
}

// initialise template functions
//-----------------------------------------------------------------------------
template string to_string< int >    ( const int&    );
template string to_string< float >  ( const float&  );

//-----------------------------------------------------------------------------
void printText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glRasterPos2i( nX, nY );

    for( p=pszText, lines=0; *p; p++ )
    {
        if( *p == '\n' )
        {
            lines++;
            glRasterPos2i( nX, nY-(lines*18) );
        }

        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12,  *p );
    }
}

//-----------------------------------------------------------------------------
void printBigText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glRasterPos2i( nX, nY );

    for( p=pszText, lines=0; *p; p++ )
    {
        if( *p == '\n' )
        {
            lines++;
            glRasterPos2i( nX, nY-(lines*18) );
        }

        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18,  *p );
    }
}

//-----------------------------------------------------------------------------
void setColor( const Color& color )
{
   glColor3f( color.R, color.G, color.B ); 
}

//-----------------------------------------------------------------------------
void setColor( const Color* color )
{
   if( color )
       glColor3f( color->R, color->G, color->B ); 
}

//-----------------------------------------------------------------------------
void setColor( shared_ptr< Color > color_)
{
   if( color_ )
       glColor3f( color_->R, color_->G, color_->B ); 
}

//-----------------------------------------------------------------------------
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

// returns the dot product between P1( x1, y1 ) and P2( x2, y2 )
//-----------------------------------------------------------------------------
GLfloat dot( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    return( x1 * x2 + y1 * y2 );
}

// convert `degrees` to radians
//------------------------------------------------------------------------------
GLfloat degrees_to_radians( GLfloat degrees )
{
    double const PI = 3.14159265358979323;
    return degrees * ( PI/180.0 ); 
}

// Returns a color that represents `percentage` depending on size - the closer
// to 100, the greener; the closer to 0, the redder.
//------------------------------------------------------------------------------
shared_ptr< Color > getColorAt( int percentage )
{
    int R, G, B;
    if( percentage <= 50 )
    {
        R = 255;
        G = percentage * 255 / 50;
    }
    else if( percentage > 50 )
    {
        R = 255 + (50 - percentage ) * 255 / 50;
        G = 255;
    }
    B = 0;

    return shared_ptr< Color >( new Color( R * 100 / 255, G * 100 / 255, B * 100 / 255 ) );
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
#endif /* UTILS_CPP */
