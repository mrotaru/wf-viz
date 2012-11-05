#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
using namespace std;

#ifdef _MSC_VER
#include <windows.h>
#endif

#if defined (__WIN32__)
    #include "platform/win32.h"
#elif __linux
    #include "platform/linux-gtk.h"
#endif

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

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
#include "xml_data.h"
using namespace xmx;

#include <GL/glu.h>
#include <GL/glut.h>

shared_ptr< XMLData > data;

vector < shared_ptr<Window> > windows;
shared_ptr< Label > label1;
shared_ptr< Window > focused_window;
shared_ptr< Window > hovered_window;
shared_ptr< MapDisplay > map_display;
shared_ptr< Control > drag_started_on;
shared_ptr< CheckBox > render_filled_polygons;
shared_ptr< Label > year_display;

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
void load_shapefile_clicked()
{
    string filename = browseFile( "Shapefiles *.shp,*.shx\n*.shp;*.shx\nAll Files *.*\n*.*\n\n" );
    cout <<"filename: "<< filename << endl;
    if( !filename.empty() )
        map_display -> loadFromShapefile( filename );
    cout << "loaded xml" << endl;
}

//------------------------------------------------------------------------------
void load_xml_clicked()
{
    string filename = browseFile( "XML files *.xml\n*.xml\nAll Files *.*\n*.*\n\n" );
    if( !filename.empty() )
    {
        data = shared_ptr< XMLData >( new XMLData() );
        data -> load( filename );
        map_display -> setData( data );
        year_display -> setText( "Year: " + to_string( map_display -> getYear()) );
    }
}

void zoom_in_clicked()          { map_display -> setScale( map_display -> getScale() + 0.05f ); }
void zoom_out_clicked()         { map_display -> setScale( map_display -> getScale() - 0.05f ); }
void zoom_reset_clicked()       { map_display -> setScale( 1.0f ); map_display -> setMapOffsetX( 0 ); map_display -> setMapOffsetY( 0 ); }

void year_plus_clicked()        
{ 
    map_display -> setYear( map_display -> getYear() + 1 );
    year_display -> setText( "Year: " + to_string( map_display -> getYear()) );
}

void year_minus_clicked()
{
    map_display -> setYear( map_display -> getYear() - 1 );
    year_display -> setText( "Year: " + to_string( map_display -> getYear()) );
}

void filled_polygons_change( bool checked )   { map_display -> setDisplayFilledPolygons( checked ); }

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
    auto load_shapefile = shared_ptr< Button >( new Button( 100, 22, "Load shapefile" ) );
    load_shapefile -> setOnClick( load_shapefile_clicked );
    auto load_xml = shared_ptr< Button >( new Button( 100, 22, "Load XML data" ) );
    load_xml -> setOnClick( load_xml_clicked );
    auto zoom_in  = shared_ptr< Button >( new Button( 80, 22, "Zoom In"  ) );
    zoom_in -> setOnClick( zoom_in_clicked );
    auto zoom_out = shared_ptr< Button >( new Button( 80, 22, "Zoom Out" ) );
    zoom_out -> setOnClick( zoom_out_clicked );
    auto zoom_reset = shared_ptr< Button >( new Button( 80, 22, "Reset" ) );
    zoom_reset -> setOnClick( zoom_reset_clicked );
    auto render_filled_polygons = shared_ptr< CheckBox >( new CheckBox( 200, 18, "render filled polygons" ) );
    render_filled_polygons -> setOnChange( filled_polygons_change );
    auto year_plus  = shared_ptr< Button >( new Button( 22, 22, "+" ) );
    auto year_minus = shared_ptr< Button >( new Button( 22, 22, "-" ) );
    year_plus -> setOnClick( year_plus_clicked );
    year_minus -> setOnClick( year_minus_clicked );
    year_display = shared_ptr< Label >( new Label( 60, 22, "Year: N/A" ) );

    string help_text = 
        " Click 'Load Shapefile' and select a shapefile to load."
        "\nIf the window showing the map is selected, you can use '+'"
        "\nand '-' to zoom in and out, and the arrow keys to pan the map.";
    auto help = shared_ptr< Label >( new Label( 280, 10, help_text ) );

    string help_text_2 =
        " After a shapefile is loaded, an XML file with data can be"
        "\nloaded. It must have the format of XML files downloaded from"
        "\nthe World Bank website. If the shapefile has an index.xml, and"
        "\nthe country codes in it are available in the data file, the"
        "\ndata will be processed when displaying the map.";
    auto help2 = shared_ptr< Label >( new Label( 280, 30, help_text_2 ) );

    // map
    map_display  = shared_ptr< MapDisplay >( new MapDisplay( 380, 370, "World Map" ) );

    // put controls on windows
    window1 -> addControl( help,              4,  22 );
    window1 -> addControl( load_shapefile,    9,  80 );
    window1 -> addControl( help2,             4, 185 );
    window1 -> addControl( load_xml,          9, 288 );
    window2 -> addAutoSizedControl( map_display,   4,  25, 4, 55 );
    window2 -> addControl( zoom_in,       4, 400 );
    window2 -> addControl( zoom_out,     80, 400 );
    window2 -> addControl( zoom_reset,  180, 400 );
    window2 -> addControl( render_filled_polygons, 4, 424 );
    window2 -> addControl( year_plus        , 374, 424 );
    window2 -> addControl( year_minus       , 350, 424 );
    window2 -> addControl( year_display     , 320, 400 );

    // initt windowing
    windows.push_back( window1 );
    windows.push_back( window2 );
    window1->giveFocus();
    focused_window = window1;
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
