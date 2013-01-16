#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std; 
#ifdef _MSC_VER
#include <windows.h>
#endif

#include <GL/glu.h>

#include "geometry/point.h"
#include "geometry/line.h"
#include "utils.h"

namespace xmx {

//------------------------------------------------------------------------------
Line::Line( float _x1, float _y1, float _x2, float _y2 ):
    x1( (GLfloat )_x1 ),
    y1( (GLfloat )_y1 ),
    x2( (GLfloat )_x2 ),
    y2( (GLfloat )_y2 )
{
}

//------------------------------------------------------------------------------
Line::Line( int _x1, int _y1, int _x2, int _y2 ):
    x1( (GLfloat )_x1 ),
    y1( (GLfloat )_y1 ),
    x2( (GLfloat )_x2 ),
    y2( (GLfloat )_y2 )
{
}

//------------------------------------------------------------------------------
Line::Line( const Point& P1, const Point& P2 )
{
    x1 = P1.getX();
    y1 = P1.getY();
    x2 = P2.getX();
    y2 = P2.getY();
}

//------------------------------------------------------------------------------
Line::~Line()
{
}

//------------------------------------------------------------------------------
void Line::scale( GLfloat x_dir, GLfloat y_dir )
{

}

//------------------------------------------------------------------------------
void Line::move( GLfloat x_dir, GLfloat y_dir )
{
    x1 += x_dir;
    y1 += y_dir;
    x2 += x_dir;
    y2 += y_dir;
}

//------------------------------------------------------------------------------
void Line::rotate( GLfloat theta, GLfloat x, GLfloat y)
{
    Point P1( x1, y1 );
    P1.rotate( theta, x, y );
    x1 = P1.getX();
    y1 = P1.getY();

    Point P2( x2, y2 );
    P2.rotate( theta, x, y );
    x2 = P2.getX();
    y2 = P2.getY();
}

//------------------------------------------------------------------------------
void Line::rotate( GLfloat theta )
{
    GLfloat x_center = ( (getMaxX() - getMinX())/2 + getMinX() );
    GLfloat y_center = ( (getMaxY() - getMinY())/2 + getMinY() );

    Point P1( x1, y1 );
    P1.rotate( theta, x_center, y_center );
    x1 = P1.getX();
    y1 = P1.getY();

    Point P2( x2, y2 );
    P2.rotate( theta, x_center, y_center );
    x2 = P2.getX();
    y2 = P2.getY();
}

//------------------------------------------------------------------------------
void Line::draw()
{
    glBegin( GL_LINES );
        glVertex2f( x1, y1 );
        glVertex2f( x2, y2 );
    glEnd();
}

//------------------------------------------------------------------------------
void Line::print()
{
    cout<<" Line @ "<<this<<": ";
    cout<< " x1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x1;
    cout<< " y1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y1;
    cout<< " x2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x2;
    cout<< " y2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y2;
    cout<<endl;
}

//------------------------------------------------------------------------------
void Line::print( std::string str )
{
    cout<< str ;
    cout<< " x1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x1;
    cout<< " y1: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y1;
    cout<< " x2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<x2;
    cout<< " y2: "<< setfill(' ') << setw( 10 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) <<y2;
    cout<< endl << "length: " << dist( x1, y1, x2, y2 );
    cout<<endl;
}

// adapted from: http://stackoverflow.com/a/1501725
//------------------------------------------------------------------------------
GLfloat Line::distanceTo( GLfloat x, GLfloat y )
{
    const GLfloat l2 = pow( dist( x1, y1, x2, y2 ), 2);  // i.e. |w-v|^2 -  avoid a sqrt
    if( l2 == 0.0 ) return dist( x1, y1, x2, y2 );

    const GLfloat t = dot( x - x1, y - y1, x2 - x1, y2 - y1 ) / l2;

    if     ( t < 0.0 ) return dist( x, y, x1, y1 );     // Beyond the 'v' end of the segment
    else if( t > 1.0 ) return dist( x, y, x2, y2 );     // Beyond the 'w' end of the segment
    
    const GLfloat projection_x = x1 + t * ( x2 - x1 );  // Projection falls on the segment
    const GLfloat projection_y = y1 + t * ( y2 - y1 );
    return dist( x, y, projection_x, projection_y );
}

//------------------------------------------------------------------------------
GLfloat Line::getLength()
{
    return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

GLfloat Line::getMaxX() { return ( x1 > x2 ? x1 : x2 ); }
GLfloat Line::getMinX() { return ( x1 < x2 ? x1 : x2 ); }
GLfloat Line::getMaxY() { return ( y1 > y2 ? y1 : y2 ); }
GLfloat Line::getMinY() { return ( y1 < y2 ? y1 : y2 ); }

} // namespace xmx
