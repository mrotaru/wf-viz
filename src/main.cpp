#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <limits>
using namespace std;

#ifdef _MSC_VER
#include <windows.h>
#endif

#if defined (__WIN32__)
    #include "platform/win32.h"
#endif

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "boost/assign.hpp"

#include "globals.h"
#include "utils.h"
//#include "Point.h"
//#include "Line.h"
//#include "BezierCurve.h"
//#include "Shape.h"
//#include "Group.h"
#include "Layer.h"
#include "window.h"
#include "label.h"
#include "button.h"
#include "mapdisplay.h"
#include "checkbox.h"
#include "data_utils.h"
using namespace xmx;

#include <GL/glu.h>
#include <GL/glut.h>

// data - will be loaded from an xml file
map< string, shared_ptr< map< int, double > > > data;

vector < shared_ptr<Window> > windows;
shared_ptr< Label > label1;
shared_ptr< Window > focused_window;
shared_ptr< Window > hovered_window;
shared_ptr< MapDisplay > map_display;
shared_ptr< Control > drag_started_on;
shared_ptr< CheckBox > render_filled_polygons;

shared_ptr< Control > dragged_control = nullptr;
int drag_offset_x;
int drag_offset_y;

string VERSION    = "?";
string BUILD_ID   = "?";
string BUILD_TIME = "?";
string build_info = "";

// holds the map of the world
//Group world;

//------------------------------------------------------------------------------
void gl_init()
{
    glClearColor( 0.9, 0.9, 0.9, 0.0 );

    // initialize viewing values
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, GLUT_WINDOW_HEIGHT, 0, GLUT_WINDOW_WIDTH );
}

//------------------------------------------------------------------------------
void button_clicked()
{
    string filename = browseFile();
    if( !filename.empty() )
        map_display -> loadFromShapefile( filename );
}

void zoom_in_clicked()          { map_display -> setScale( map_display -> getScale() + 0.05f ); }
void zoom_out_clicked()         { map_display -> setScale( map_display -> getScale() - 0.05f ); }
void zoom_reset_clicked()       { map_display -> setScale( 1.0f ); map_display -> setMapOffsetX( 0 ); map_display -> setMapOffsetY( 0 ); }
void filled_polygons_change( bool checked )   { map_display -> setDisplayFilledPolygons( checked ); }

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
void loadXMLData( string file_name )
{
    using boost::property_tree::ptree;
    ptree pt;

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
    cout << data[ "ROU" ]->size() << endl;
    map< int, double > map_ = *(data[ "ROU" ]);
    cout << map_[ 1987 ] << endl;
}

// The following functions are used to interrogate the `data` map. It is assumed
// the are called after the xml file has been loaded. They should really be
// encapsulated in a class, with `data` as a private field, instead of cluttering
// main.cpp, but there's not time...
//------------------------------------------------------------------------------
double getMaxDataValue( int year )
{
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
double getMinDataValue( int year )
{
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
int getMaxDataYear()
{
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
int getMinDataYear()
{
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
double getValueForYear( string iso3_code, int year )
{
    map< int, double > map_ = *( data[ iso3_code ] );
    return map_[ year ];
}

// Perform application-specific initialisation
//------------------------------------------------------------------------------
void app_init()
{
    // read version number and build info
    ifstream ver_file( "VERSION", ifstream::in );
    getline( ver_file, VERSION );
    VERSION = "World Statistics, " + VERSION;
    getline( ver_file, BUILD_ID );
    getline( ver_file, BUILD_TIME );
    build_info = "Build info: " + BUILD_ID + " @ " + BUILD_TIME;

    // create a couple of windows
    auto window1 = shared_ptr< Window >( new Window(  40,  70, 400, 400, "Window #1" ) );
    auto window2 = shared_ptr< Window >( new Window( 450,  70, 400, 450, "Map of the world" ) );

    // controls
    label1 = shared_ptr< Label >( new Label( 200, 18, "This is a label" ) ); 
    auto checkbox1 = shared_ptr< CheckBox >( new CheckBox( 200, 18, "test a checkbox" ) );
    auto btn = shared_ptr< Button >( new Button( 100, 22, "Load shapefile" ) );
    btn->setOnClick( button_clicked );
    auto zoom_in  = shared_ptr< Button >( new Button( 80, 22, "Zoom In"  ) );
    zoom_in -> setOnClick( zoom_in_clicked );
    auto zoom_out = shared_ptr< Button >( new Button( 80, 22, "Zoom Out" ) );
    zoom_out -> setOnClick( zoom_out_clicked );
    auto zoom_reset = shared_ptr< Button >( new Button( 80, 22, "Reset" ) );
    zoom_reset -> setOnClick( zoom_reset_clicked );
    auto render_filled_polygons = shared_ptr< CheckBox >( new CheckBox( 200, 18, "render filled polygons" ) );
    render_filled_polygons -> setOnChange( filled_polygons_change );

    string help_text = 
        " Click 'Load Shapefile' and select a shapefile to load."
        "\nIf the window showing the map is selected, you can use '+'"
        "\nand '-' to zoom in and out, and the arrow keys to pan the map.";
    auto help = shared_ptr< Label >( new Label( 280, 230, help_text ) );

    // map
    map_display  = shared_ptr< MapDisplay >( new MapDisplay( 380, 370, "World Map" ) );

    // put controls on windows
    window1 -> addControl( help,   4,  22 );
    window1 -> addControl( btn,    4, 245 );
    window2 -> addAutoSizedControl( map_display,   4,  25, 4, 55 );
    window2 -> addControl( zoom_in,       4, 400 );
    window2 -> addControl( zoom_out,     80, 400 );
    window2 -> addControl( zoom_reset,  180, 400 );
    window2 -> addControl( render_filled_polygons, 4, 424 );

    // initt windowing
    windows.push_back( window1 );
    windows.push_back( window2 );
    window1->giveFocus();
    focused_window = window1;

    //
    loadXMLData( "data/NY.GDP.MKTP.CD_Indicator_en.xml" );
    for( int i = getMinDataYear(); i <= getMaxDataYear(); i++ )
    {
        getMaxDataValue( i );
        getMinDataValue( i );
    }
}

//------------------------------------------------------------------------------
void gl_display_callback()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glRenderMode( GL_RENDER );

    glColor3ub( 60, 60, 60 );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 18, VERSION );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 32, build_info );

    BOOST_FOREACH( shared_ptr< Window > window, windows )
        if( window != focused_window )
            window->draw();
    focused_window -> draw();

    glutSwapBuffers();
}

//------------------------------------------------------------------------------
void gl_reshape_callback( int nWidht, int nHeight )
{
    window_width = nWidht;
    window_height = nHeight;
    glViewport( 0, 0, (GLsizei)nWidht, (GLsizei)nHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, nWidht, 0, nHeight);
    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void gl_mouse_click_callback( int button, int state, int x, int y )
{
    label1->setText( "click: x = " + to_string( x ) + ", y = " + to_string( y ) +
            ", button: " + to_string( button ) + ", state: " + to_string( state )); 
    glutPostRedisplay();

    if( state ) drag_started_on = shared_ptr< Control >();
    if( state && dragged_control )
        dragged_control = shared_ptr< Control >();

    BOOST_FOREACH( shared_ptr< Window > window, windows )
    {
        if( window->isPointInside( x, y ) )
        {
            focused_window->losfFocus();
            window->giveFocus();
            focused_window = window;
            if( !window->isPointInsideAnyControl( x - window->getX(), y - window->getY() ) )
                drag_started_on = window;
            window->clickEvent( x, y, button, state );
        }
    }
}

//------------------------------------------------------------------------------
void gl_mouse_drag_callback( int x, int y )
{
    if( dragged_control )
    {
        dragged_control->setX( x - drag_offset_x );
        dragged_control->setY( y - drag_offset_y );
        dragged_control->draw();
        glutPostRedisplay(); 
    }
    else
    {
        // dragging a window ?
        BOOST_FOREACH( auto window, windows )
            if( drag_started_on == window &&
                window -> isPointInside( x, y ) &&
               !window -> isPointInsideAnyControl( x - window -> getX(), y - window -> getY() ) )
            {
                dragged_control = window;
                drag_offset_x = x - dragged_control->getX();
                drag_offset_y = y - dragged_control->getY();
            }

        // some controls ( namely, MapDisplay ) need to know when the user drags over them
        BOOST_FOREACH( auto window, windows )
            if( window -> isPointInside( x, y ) )
                window -> dragEvent( x, y );
    }
}

//------------------------------------------------------------------------------
void gl_mouse_move_callback( int x, int y )
{
    if( dragged_control )
        dragged_control = shared_ptr< Control >();

    bool hovering_a_window = false;
    BOOST_FOREACH( shared_ptr< Window > window, windows )
    {
        if( window->isPointInside( x, y ) )
        {
            hovered_window = window;
            hovering_a_window = true;
            window->hoverEnterEvent( x, y );
        }
    }

    if( !hovering_a_window && hovered_window )
        hovered_window = shared_ptr< Window >();
}

//------------------------------------------------------------------------------
void gl_keyboard_callback( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 27:
            exit(0);
            break;
    }

    focused_window -> keyPressed( key, x, y );
}

//------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE );
     
    // window
    window_width = 900;
    window_height = 600;
    glutInitWindowSize( window_width, window_height );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "World Factbook" );

    gl_init();  // OpenGL-specific initializations
    app_init(); // application-specific initializations

    // callbacks
    glutDisplayFunc         ( gl_display_callback );
    glutKeyboardFunc        ( gl_keyboard_callback );
    glutReshapeFunc         ( gl_reshape_callback );
    glutMouseFunc           ( gl_mouse_click_callback );
    glutMotionFunc          ( gl_mouse_drag_callback );
    glutPassiveMotionFunc   ( gl_mouse_move_callback );
    glutSpecialFunc         ( gl_keyboard_callback );

    glutMainLoop();
    
    return 0;
}
