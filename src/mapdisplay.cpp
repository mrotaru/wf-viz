#include <iostream>
#include <vector>
#include <cmath>
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

//------------------------------------------------------------------------------
void MapDisplay::draw()
{
    Control::draw();
    int total_width  = abs( map_BB.minX ) + abs( map_BB.maxX );

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
            glVertex2f( point.x, toGl( point.y ) );
        }
        glEnd();
    }

    setColor( RED );
    int parent_x = parent->getX();
    int parent_y = parent->getY();
    int i=1;
    BOOST_FOREACH( LineString2D polygon, polygons )
    {
        setColor( i==209 ? GREEN: RED );
        glBegin( GL_POLYGON );
        BOOST_FOREACH( Point2D point, polygon.points )
        {
            int py = point.y;
            int _y = 0;
            if( py > 0 )
               _y = abs(map_BB.maxY) - py;
            else
               _y = abs(map_BB.maxY) + abs(py);
            glVertex2f( parent_x + x + (point.x + total_width/2),
                        toGl( parent_y + y + _y ) );
        }
        glEnd();
        i++;
    }
    glFlush();
}

//------------------------------------------------------------------------------
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
    else if( hSHP -> nShapeType == SHPT_POLYGON ) //Polygon Shapefile
    {
        cout << "shapte type = SHPT_POLYGON" << endl;
        cout << hSHP -> nRecords << " polygons. " << endl;
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
