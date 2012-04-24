#include <iostream>
#include <vector>
using namespace std;

#include <GL/glu.h>
#include <GL/glut.h>

#include <boost/foreach.hpp>

#include "shapefil.h"

#include "window.h"
#include "button.h"
#include "gui_utils.h"
#include "mapdisplay.h"

namespace xmx
{

typedef struct Point2D      { double x; double y;           } Point2D;
typedef struct LineString2D { vector< Point2D > points;     } LineString2D;
typedef struct BoundingBox  { float maxX, maxY, minX, minY; } BoundingBox;

vector< Point2D >       points;
vector< LineString2D >  lines;
vector< LineString2D >  polygons;
BoundingBox             map_BB;

void MapDisplay::draw()
{
    Control::draw();

//    gluOrtho2D( map_BB.minX, map_BB.maxX, map_BB.minY, map_BB.maxY );
//    gluOrtho2D( x, x + width, y, y + height );
//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity();
//    glViewport( parent -> getX() + x, parent -> getY() + y, width, height );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( 100.0f, 100.0f, 0.0f );

	//Render Point Shapefile
	setColor    ( BLUE );
	glEnable    ( GL_POINT_SMOOTH );
	glPointSize ( 5.0 );

	glBegin     ( GL_POINTS );
    BOOST_FOREACH( Point2D point, points )
    {
        glVertex2f( point.x, point.y );
    }
	glEnd();
	
    setColor( GREEN );
    BOOST_FOREACH( LineString2D line, lines )
    {
        glBegin( GL_LINE_STRIP );
        BOOST_FOREACH( Point2D point, line.points )
        {
            glVertex2f( point.x, point.y );
        }
        glEnd();
    }

    int shown = 0;
    setColor( RED );
    BOOST_FOREACH( LineString2D polygon, polygons )
    {
        glBegin( GL_LINE_LOOP );
        BOOST_FOREACH( Point2D point, polygon.points )
        {
            glVertex2f( point.x, point.y );
        }
        glEnd();
    }

    glFlush();
}

void MapDisplay::loadFromShapefile( std::string filename )
{
    SHPHandle hSHP = SHPOpen( filename.c_str(), "rb" );

    if( hSHP == NULL )
    {
        file_loaded = false;
        return;
    }

    file_loaded = true;

    map_BB.maxX = hSHP -> adBoundsMax[0]; cout << "maxX: " << map_BB.maxX << endl;
    map_BB.maxY = hSHP -> adBoundsMax[1]; cout << "maxY: " << map_BB.maxY << endl;
    map_BB.minX = hSHP -> adBoundsMin[0]; cout << "minX: " << map_BB.minX << endl;
    map_BB.minY = hSHP -> adBoundsMin[1]; cout << "minY: " << map_BB.minY << endl;

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
    else if(hSHP->nShapeType == SHPT_POLYGON) //Polygon Shapefile
    {
        cout << "shapte type = SHPT_POLYGON" << endl;
		SHPObject *psShape;
		for( int i=0; i < hSHP -> nRecords; i++ )
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
			LineString2D polygon;
			polygon.points = tempPointArray;
			polygons.push_back( polygon );
		}
    }
}

} // namespace xmx
