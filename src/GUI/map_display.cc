#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

#include <GL/glu.h>
#include <GL/glut.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "shapefil.h"

#include "utils.h"

#include "GUI/window.h"
#include "GUI/button.h"
#include "GUI/gui_utils.h"
#include "GUI/map_display.h"

namespace xmx
{

//------------------------------------------------------------------------------
void MapDisplay::setData( shared_ptr< XMLData > data_ )
{
    data = data_;
    have_data = true;
    year = data -> getMinDataYear();
}

//------------------------------------------------------------------------------
void MapDisplay::draw()
{
    Control::draw();

    if( !file_loaded ) return;

    int x_middle = parent->getX() + x + map_width/2;
    int y_middle = window_height - parent->getY() - y - map_height/2;

    glMatrixMode( GL_MODELVIEW ); 
    glTranslated(  -1 * ( x_middle * scale - x_middle ), -1 * ( y_middle * scale - y_middle ), 0.0f );
    glTranslated( map_offset_x, map_offset_y, 0.0f );
    glScalef( scale, scale, 1.0f );

	// render point shapefile
    //------------------------------------------------------------------------------
	setColor    ( BLUE );
	glEnable    ( GL_POINT_SMOOTH );
	glPointSize ( 5.0 );

	glBegin     ( GL_POINTS );
    BOOST_FOREACH( Point2D point, points )
    {
        glVertex2f( point.x, point.y );
    }
	glEnd();
	
    // render line shapefile
    //------------------------------------------------------------------------------
    setColor( GREEN );
    BOOST_FOREACH( LineString2D line, lines )
    {
        glBegin( GL_LINE_STRIP );
        BOOST_FOREACH( Point2D point, line.points )
        {
            glVertex2f( point.x, toGl( point.y ) );
        }
        glEnd();
    }

    // render polygon shapefile
    //------------------------------------------------------------------------------
    int parent_x = parent->getX();
    int parent_y = parent->getY();

    glEnable ( GL_SCISSOR_TEST );
    glScissor( parent_x + x, toGl( parent_y + y + height ), width, height );
    setColor( RED );

    double max_for_year = 0;
    double min_for_year = 0;
    if( have_data )
    {
        max_for_year = data -> getMaxDataValue( year );
        min_for_year = data -> getMinDataValue( year );
    }

    int i=1;
    BOOST_FOREACH( LineString2D polygon, polygons )
    {
        // Each `polygon` contains the data for a particular country.
        // They are stored in the order of their index, so the first country
        // is the country which in the index file has an index of 1.
        //--------------------------------------------------------------------------------
        if( have_index )
        {
            bool not_available = false; // 
            float proportion = 0;

            // get the ISO3 code for the current country
            string current_iso3 = index[ i ];

            if( have_data && data -> isLoaded() )
            {
                double value = data -> getValueForYear( current_iso3, year );
                if( value == -1 )
                    not_available = true;
                proportion = value / max_for_year;
            }
            if( not_available )
                setColor( DIM_GREY ); 
            else
            {
                shared_ptr< Color > c = getColorAt( proportion );
                setColor( c ); 
            }
        }

        // draw the country
        //--------------------------------------------------------------------------------
        glBegin( filled_polygons ? GL_POLYGON : GL_LINE_STRIP );
        BOOST_FOREACH( Point2D point, polygon.points )
        {
            glVertex2f( parent_x + x + point.x,
                  toGl( parent_y + y + point.y ) );
        }
        glEnd();
        i++;
    }
    glDisable( GL_SCISSOR_TEST );

    glLoadIdentity();

    setColor( BLACK );
    printText( parent_x + x + 2, toGl( parent_y + y + 10 ), "x offset: " + to_string( map_offset_x ) );
    printText( parent_x + x + 2, toGl( parent_y + y + 22 ), "y offset: " + to_string( map_offset_y ) );
}

//------------------------------------------------------------------------------
void MapDisplay::dragEvent( int x_, int y_ )
{
//    cout << "drag event in MapDisplay: " << x_ << ", " << y_ << endl;
    map_offset_x = x_ - map_offset_x;
    map_offset_y = y_ - map_offset_y;
    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void MapDisplay::keyPressed( unsigned char key, int x_, int y_ )
{
    switch( key )
    {
        case GLUT_KEY_UP:
            map_offset_y -= 5;
            break;
        case GLUT_KEY_DOWN:
            map_offset_y += 5;
            break;
        case GLUT_KEY_LEFT:
            map_offset_x += 5;
            break;
        case GLUT_KEY_RIGHT:
            map_offset_x -= 5;
            break;
        case '=':
        case '+':
            scale += 0.5f;
            break;
        case '-':
            scale -= 0.5f;
            break;
        case '0':
            scale = 1;
            break;
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void MapDisplay::loadFromShapefile( std::string filename )
{
    shapefile = filename;
    SHPHandle hSHP = SHPOpen( filename.c_str(), "rb" );

    if( hSHP == NULL )
    {
        file_loaded = false;
        return;
    }

    file_loaded = true;
    cout << "reading shapefile..." << endl;

    map_BB.maxX = hSHP -> adBoundsMax[0]; cout << "maxX: " << map_BB.maxX << endl;
    map_BB.maxY = hSHP -> adBoundsMax[1]; cout << "maxY: " << map_BB.maxY << endl;
    map_BB.minX = hSHP -> adBoundsMin[0]; cout << "minX: " << map_BB.minX << endl;
    map_BB.minY = hSHP -> adBoundsMin[1]; cout << "minY: " << map_BB.minY << endl;
    map_width  = abs( map_BB.minX ) + abs( map_BB.maxX );
    map_height = abs( map_BB.minY ) + abs( map_BB.maxY );

    if( hSHP -> nShapeType == SHPT_POINT ) //Point Shapefile
    {
        cout << "shapte type = SHPT_POINT" << endl;
        SHPObject *psShape;
        for( int i=0;i < hSHP -> nRecords; i++ )
        {
            psShape = SHPReadObject( hSHP, i );

            double x =  psShape -> padfX[0];
            double y = -psShape -> padfY[0];

            //Plot these points
            Point2D pt;
            pt.x =  x;
            pt.y = -y;
            points.push_back( pt );
        }
    }
    else if( hSHP->nShapeType == SHPT_ARC ) //Line Shapefile
    {
        cout << "shapte type = SHPT_ARC" << endl;
        SHPObject *psShape;
	    for( int i=0; i < hSHP -> nRecords; i++)
        {
            psShape = SHPReadObject( hSHP, i );
            vector< Point2D > tempPointArray;

            for( int j=0; j < psShape -> nVertices; j++ )
            {
				double x = psShape -> padfX[j];
				double y = psShape -> padfY[j];
				Point2D pt;
				pt.x = x;
				pt.y = y;
				tempPointArray.push_back( pt );
            }
           
			LineString2D linestring;
            linestring.points = tempPointArray;
            lines.push_back( linestring );
        }
    }
    else if( hSHP -> nShapeType == SHPT_POLYGON ) //Polygon Shapefile
    {
        cout << "shape type = SHPT_POLYGON" << endl;
        cout << hSHP -> nRecords << " polygons. " << endl;
        double absMinY = abs( map_BB.minY );
        double absMaxY = abs( map_BB.maxY );
        double absMinX = abs( map_BB.minX );
        //double absMaxX = abs( map_BB.maxX );

		SHPObject *psShape;
		for( int i=0; i < hSHP -> nRecords; i++ )
		{
			psShape = SHPReadObject( hSHP, i );
      		vector< Point2D > tempPointArray;
     
			for( int j=0; j < psShape -> nVertices; j++ )
			{
				double sx = psShape -> padfX[j];
				double sy = psShape -> padfY[j];
				Point2D pt;
				
                //------------------------------------------------------------------------
                //      MapDisplay control
                //      .-----------------------.
                //      |          | pt.y       |
                //      |  pt.x    V            |
                //      |--------->X (pt)       |
                //      "-----------------------"
                //------------------------------------------------------------------------
                pt.x = sx > 0 ? absMinX + sx : abs( absMinX ) - abs( sx );
				pt.y = sy > 0 ? map_height - sy - absMinY : abs( absMaxY ) + abs( sy );

      			tempPointArray.push_back( pt );
			}
			LineString2D polygon;
			polygon.points = tempPointArray;
			polygons.push_back( polygon );
		}
        cout << "done reading polygon shapefile." << endl;
    }

    // look for an index file - an xml file with data of the form:
    //------------------------------------------------------------------------------
    //      <shape>
    //          <index>1</index>
    //          <id>ROU</id>
    //      </shape>
    //------------------------------------------------------------------------------
#if defined (__WIN32__)
    int last_slash = shapefile.find_last_of( '\\' );
    int last_dot   = shapefile.find_last_of( '.'  );
    string base_name = shapefile.substr( last_slash + 1, last_dot - last_slash - 1 );
    string folder_name = shapefile.substr( 0, last_slash );
    string index_file_name = folder_name + "\\" + base_name + ".xml";
#elif __linux
    int last_slash = shapefile.find_last_of( '/' );
    int last_dot   = shapefile.find_last_of( '.'  );
    string base_name = shapefile.substr( last_slash + 1, last_dot - last_slash - 1 );
    string folder_name = shapefile.substr( 0, last_slash );
    string index_file_name = folder_name + "/" + base_name + ".xml";
#endif

    //-------------------------------------------------------------------------------
    ifstream index_file( index_file_name );
    if( !index_file.good() )
    {
        cout << "warning: cannot find an index file for " << base_name << "." << endl;
    }
    else
    {
        index_file.close();

        using boost::property_tree::ptree;
        ptree pt;

        cout << endl << "reading index file..." << endl;
        read_xml( index_file_name, pt );

        BOOST_FOREACH( ptree::value_type const& v, pt.get_child( "records" ) )
        {
            string country_name  = v.second.get< string >( "name"  );
            string country_code  = v.second.get< string >( "iso3"  );
            int    country_index = v.second.get< int    >( "index" );

            cout << "ISO3: " << country_code;
            cout << " ( " << country_name  << " )";
            cout << " index: " << country_index << endl;

            index[ country_index ] = country_code;
        }
        cout << "done reading index file." << endl;
        have_index = true;
    }
}

} // namespace xmx
