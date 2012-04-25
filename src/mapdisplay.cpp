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

void MapDisplay::draw()
{
    Control::draw();
    int total_width  = abs( map_BB.minX ) + abs( map_BB.maxX );
    int total_height = abs( map_BB.minY ) + abs( map_BB.maxY );

//    cout << "width:  " << total_width  << endl;
//    cout << "height: " << total_height << endl;

//    glMatrixMode( GL_MODELVIEW );
//    glTranslatef( total_width/2, total_height/2, 0.0f );

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

    int shown = 0;
    setColor( RED );
    int parent_x = parent->getX();
    int parent_y = parent->getY();
    int parent_width = parent->getWidth();
    int parent_height = parent->getHeight();
    int miny =  1000;
    int maxy = -1000;
    BOOST_FOREACH( LineString2D polygon, polygons )
    {
        glBegin( GL_LINE_LOOP );
        BOOST_FOREACH( Point2D point, polygon.points )
        {
            int px = point.x;
            int py = point.y;
//            int y_coord = parent_y + y + (point.y < 0 ? abs(point.y) : abs(map_BB.minY) + point.y) ;
//            int y_coord = parent_y + y + point.y;
            int y_coord = py;

            maxy = py > maxy ? py : maxy;
            miny = py < miny ? py : miny;
//            if( shown<=1 && point.y < 0 )
//            {
//                cout << "---------------------------------------" << endl;
//                cout << "point.y:               " << point.y << endl;
//                cout << "toGL(point.y):         " << toGl(point.y) << endl;
//                cout << "toGL( abs(point.y) ):  " << toGl(abs(point.y)) << endl;
//                cout << "parent_y:              " << parent_y << endl;
//                cout << "y:                     " << y << endl;
//                cout << "y_coord:               " << y_coord << endl;
//                shown++;
//            }
            int _y = 0;
            if( py > 0 )
               _y = abs(map_BB.maxY) - py;
            else
               _y = abs(map_BB.maxY) + abs(py);
            glVertex2f( parent_x + x + (point.x + total_width/2),
//                        toGl( parent_y + (parent_height - y - (point.y + total_height)) ) );
                        toGl( parent_y + y + _y ) );
        }
        glEnd();
    }

//    cout << "miny: " << miny << endl;
//    cout << "maxy: " << maxy << endl;

//    glLoadIdentity();
//    glFlush();
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
    else if( hSHP -> nShapeType == SHPT_POLYGON ) //Polygon Shapefile
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
