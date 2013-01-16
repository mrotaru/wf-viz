#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

#include <GL/glu.h>
#include <GL/glut.h>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "utils.h"
#include "geometry/point.h"
#include "geometry/bezier_curve.h"
#include "geometry/shape.h"
#include "geometry/group.h"
using namespace xmx;

// disable 'comparison between signed and unsigned integer expressions'
#pragma GCC diagnostic ignored "-Wsign-compare"

typedef boost::shared_ptr<Shape> sptrShape;

namespace xmx {

Group::Group() : color( dcol ), lineWidth( 1.0f )
{
}

Group::~Group()
{
}

void Group::setColor( GLfloat R, GLfloat G, GLfloat B )
{
    color.R = R;
    color.G = G;
    color.B = B;
    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> color.R = R;
        sptr_shape -> color.G = G;
        sptr_shape -> color.B = B;
    }
}

void Group::setColor( Color& new_color )
{
    color.R = new_color.R;
    color.G = new_color.G;
    color.B = new_color.B;

    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> color.R = new_color.R;
        sptr_shape -> color.G = new_color.G;
        sptr_shape -> color.B = new_color.B;
    }
}

void Group::draw()
{
    glColor3f( color.R, color.G, color.B );
    glLineWidth( lineWidth );

    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> draw();
    }
    
    glColor3f( dcol.R, dcol.G, dcol.B );
    glLineWidth( lineWidth );
}


void Group::move( GLfloat x_dir, GLfloat y_dir )
{
    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> move( x_dir, y_dir );
    }
}


void Group::scale( GLfloat x_dir, GLfloat y_dir )
{
    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> scale( x_dir, y_dir );
    }
}


void Group::rotate( GLfloat theta )
{
    GLfloat center_x = (( getMaxX() - getMinX() )/2 + getMinX());
    GLfloat center_y = (( getMaxY() - getMinY() )/2 + getMinY());

    BOOST_FOREACH( boost::shared_ptr< Shape > sptr_shape, shapes )
    {
        sptr_shape -> rotate( theta, center_x, center_y );
    }
}

void Group::addShape( Shape& shape )
{
    return /* something */;
}

// assumes `line` is a string containing the coordinates 4 points, something like:
// /*   1*/ <140.00000000, 440.00000000>, <210.00000000, 370.00000000>, <490.00000000, 560.00000000>, <400.00000000, 410.00000000>,
// this function parses lines of this type, returning a shared_ptr to the Bezier curved described by the points
// specified on the line.
boost::shared_ptr< BezierCurve > parseBezierCurve( string line )
{
    bool DPL = DEBUG_POV_LOADING;
    if( DPL )cout << "parsing line " << line <<" for a Bezier curve" << endl;

    static const boost::regex split_coords_regex ( ">, <|/ <|>,$|>$", boost::regex::extended );
    static const boost::regex curve_no_regex     ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );
    static const boost::regex comma_split_regex  ( ", ", boost::regex::extended );

    Point pointz[4];

    vector< string > result;

    boost::algorithm::split_regex( result, line, split_coords_regex );

    for( uint i=1; i< result.size()-1; i++ )
    {   // split the pairs into numbers
        vector<string> result2;
        boost::algorithm::split_regex ( result2, result[i], comma_split_regex );

        pointz[i-1].x = boost::lexical_cast<GLfloat>( result2[0] );
        pointz[i-1].y = boost::lexical_cast<GLfloat>( result2[1] );

        if( DEBUG_POV_LOADING ) cout<<"P"<< i-1 <<": x: "<< pointz[i-1].x <<", y: "<< pointz[i-1].y <<endl;
    }

    boost::shared_ptr< BezierCurve > mbc( new BezierCurve( pointz[0], pointz[1], pointz[2], pointz[3] ));
    return mbc;
}

// try parsing a Shape from `lines`, starting with `start_line`
// and if parsing is successufll, put the last parsed line into `end_line`
boost::shared_ptr< Shape > parseShape( vector< string >& lines, uint& line_no )
{
    bool DPL = DEBUG_POV_LOADING;
    if( DPL )cout<< "parsing Shape starting with line " << line_no << endl;
    string line = lines[ line_no ];

    static const boost::regex shape_end_regex    ( "#{3} end ([^[:space:]]+)", boost::regex::extended );
    static const boost::regex curve_regex        ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );
    static const boost::regex shape_name_regex   ( "\\s*#declare ([^[:space:]]+)\\s*=\\s*prism", boost::regex::extended );

    string shape_name = getFromRegex< string >( line, shape_name_regex );
    if( DPL )cout<< "shape name: "<< shape_name << endl;
    boost::shared_ptr< Shape > sptr_shape( new Shape());
    sptr_shape->name = shape_name;
    sptr_shape->isBezier = true;

    bool shape_finished = false;

    while( line_no <= lines.size() && !shape_finished )
    {
        if( regex_search( line, curve_regex ))
            sptr_shape->addBezierCurve( parseBezierCurve( line ));

        else if( regex_search( line, shape_end_regex ) && ( getFromRegex< string >( line, shape_end_regex ) == sptr_shape->name ))
        {
            if( DPL )cout<<"sptr_shape " << sptr_shape->name << " end at: " << line_no << endl;
            shape_finished = true;
        }
        else
        {
            if( DPL )cout<<"no bezier, no end: " << line << endl;
        }
        line_no ++;
        line = lines[ line_no ];
    }

    if( !shape_finished )
    {
        string msg = "Pov parsing error: sptr_shape " + sptr_shape->name + " not finished";
        throw runtime_error( msg );
    }

    return sptr_shape;

}

void Group::loadFromPovFile( string filename )
{

    bool DPL = DEBUG_POV_LOADING;
    //    list< sptr_shape > mlst = (list< sptr_shape >*)*shape_list;
    cout<< "loading x" << filename << " into Group @ " << this << " ( " << name << " ) ... ";
    uint total_lines = 0;
    vector<string> lines;
    lines.push_back( filename );

    // read the whole file in memory
    ifstream my_file;
    my_file.open( filename.c_str(), ifstream::in );
    if( !my_file.is_open() )
    {
        cout<<"could not find '" << filename <<"'. Press 'Enter' to exit" << endl;
        char ch;
        cin.get(ch);
        exit(1); 
    }

    string line;
    while( getline( my_file, line ))
    {
        total_lines++;
        lines.push_back( line );
    }

    if( DPL )cout<< "total lines: " << total_lines << endl;

    // process the file
    int header_start               = 0;     // the line number where the header starts

    // find the header
    uint cline = 1; // the current line
    while( !header_start && cline <= total_lines )
    {
        cline++;
        if ( lines[ cline ].find( "##   Exports in this file" ) != -1 ) 
            header_start = cline;
    }

    // no header ?
    if( !header_start && STRICT_POV )
        throw runtime_error("Pov parsing error: can't find the header");

    if( DPL )cout << "header start: " << header_start << endl;

    // setup some regexes
    static const boost::regex shapes_regex       ( "^##\\s{1,}Shapes\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex segments_regex     ( "^##\\s{1,}Segments\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex nodes_regex        ( "^##\\s{1,}Nodes\\s{1,}: (\\d{1,})$", boost::regex::extended );
    static const boost::regex curve_no_regex     ( "\\s*/\\*\\s*(\\d{1,})\\*/ <", boost::regex::extended );

    // parse the header
    if( STRICT_POV )
    {
        if( lines[ header_start + 2 ].find( "Shapes" ) == -1 )
        {
            cout<<"line: "<< header_start + 2 << endl;
            throw runtime_error("Pov parsing error: expected 'Shapes'; line: '" + lines[ header_start + 2 ] + "'" );
        }
        else
        {
            if( DPL )cout<<"getting asdasd"<< endl;
        }
    }

    if( STRICT_POV )
    {
        if( lines[ header_start + 3 ].find( "Segments" ) == -1 )
        {
            cout<<"line: "<< header_start + 3 << endl;
            throw runtime_error("Pov parsing error: expected 'Segments'; line: '" + lines[ header_start + 2 ]  + "'" );
        }
    }

    if( STRICT_POV )
    {
        if( lines[ header_start + 4 ].find( "Nodes" ) == -1 )
        {
            cout<<"line: "<< header_start + 4 << endl;
            throw runtime_error("Pov parsing error: expected 'Nodes'; line: '" + lines[ header_start + 2 ] + "'" );
        }
    }

    // we're done with the header;
    // now let's look for those shapes

    // we'll need some regex'es
    static const boost::regex shape_start        ( "\\s*#declare ((path[-\\d]{4,})|([[:digit:]]*.+))\\s*=\\s*prism", boost::regex::extended );

    bool inside_shape = false;

    // iterate over the rest of the lines
    cline = header_start + 5;                                                                                         
    while( cline <= total_lines )
    {
        if( DPL )cout<< "processing line: " << cline << " - '" << lines[ cline ] << "'" << endl;
        string line = lines[ cline ];

        // does the line contain a curve ?
        if ( regex_search( line, curve_no_regex ))
        {
            if( !inside_shape )
            {
                cout<<"line: "<< cline << endl;
                throw runtime_error( "Pov parsing error: points encoutered outside a shape definition" );
            }
        }

        // does the line define the start of a shape ?
        else if ( regex_search( line, shape_start ))
        {
            if( inside_shape )
            {

                if( DPL )cout<<"line: "<< cline << endl;
                cout<<"Pov parsing warning: nested shapes not supported ( line " << cline + 1 << endl;
                inside_shape = true;
            }
            else 
            {
                inside_shape = true;

                shapes.push_back( parseShape( lines, cline ) );
                if( DPL )cout << "parsed shape; " << cline << endl;
                inside_shape = false;
            }
        }
        cline ++;
    }

    cout<< " OK - " << shapes.size() << " shapes loaded." << endl;
}

void Group::print()
{
    BOOST_FOREACH( boost::shared_ptr< Shape > pShape, shapes )
        pShape -> print();
}


GLfloat Group::getMaxX()
{
    GLfloat maxX = shapes.front() -> getMaxX();
    BOOST_FOREACH( boost::shared_ptr< Shape > pShape, shapes )
        if( pShape -> getMaxX() > maxX ) maxX = pShape -> getMaxX();
    return maxX;
}

GLfloat Group::getMinX()
{
    GLfloat minX = shapes.front() -> getMinX();
    BOOST_FOREACH( boost::shared_ptr< Shape > pShape, shapes )
        if( pShape -> getMinX() < minX ) minX = pShape -> getMinX();
    return minX;
}

GLfloat Group::getMaxY()
{
    GLfloat maxY = shapes.front() -> getMaxY();
    BOOST_FOREACH( boost::shared_ptr< Shape > pShape, shapes )
        if( pShape -> getMaxY() > maxY ) maxY = pShape -> getMaxY();
    return maxY;
}

GLfloat Group::getMinY()
{
    GLfloat minY = shapes.front() -> getMinY();
    BOOST_FOREACH( boost::shared_ptr< Shape > pShape, shapes )
        if( pShape -> getMinY() < minY ) minY = pShape -> getMinY();
    return minY;
}
} // namespace xmx
