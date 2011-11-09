#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

#include <cmath>

#include "Point.h"

namespace xmx {

//------------------------------------------------------------------------------
void nice_cout( string msg, GLfloat f )
{
    cout<< msg << setfill(' ') << setw( 15 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) << f <<endl;
}

//------------------------------------------------------------------------------
void nice_cout_( string msg, GLfloat f )
{
    cout<< msg << setfill(' ') << setw( 15 ) <<setiosflags( ios::fixed | ios::right ) << setprecision(4) << f;
}

//------------------------------------------------------------------------------
void Point::print()
{
    cout<< setfill(' ') << setw(15) << left <<  "Point " << " @ " << this << " : ";
    nice_cout_( "x: ", x );
    cout<<" ";
    nice_cout_( "y: ", y );
    cout<<endl;
}

//------------------------------------------------------------------------------
void Point::print( string str )
{
    cout<<str<<" > ";
    nice_cout_( "x: ", x );
    cout<<" ";
    nice_cout_( "y: ", y );
    cout<<endl;
}

//------------------------------------------------------------------------------
Point psqrt( const Point& p )
{
    return Point( sqrt( p.getX() ), sqrt( p.getY() ), sqrt( p.getZ()));
}

// rotates the point `theta` degrees around the origin
//------------------------------------------------------------------------------
void Point::rotate( const GLfloat& theta )
{
    GLfloat theta_radians = theta/180 * M_PI;
    x = x * cos( theta_radians ) - y * sin( theta_radians );
    y = x * cos( theta_radians ) + y * sin( theta_radians );
}

// rotates the point `theta` degrees around `x_coord`,`y_coord` 
//------------------------------------------------------------------------------
void Point::rotate( const GLfloat& theta, const Point& P )
{
    GLfloat theta_radians = theta/180 * M_PI;
    x = P.getX() + (( x - P.getX() ) * cos( theta_radians )) - (( y - P.getY() ) * sin( theta_radians ));
    y = P.getY() + (( y - P.getY() ) * cos( theta_radians )) + (( x - P.getX() ) * sin( theta_radians ));
}


// rotates the point `theta` degrees around `x_coord`,`y_coord` 
//------------------------------------------------------------------------------
void Point::rotate( const GLfloat& theta, const GLfloat& x_coord, const GLfloat& y_coord )
{
    GLfloat theta_radians = theta/180 * M_PI;
    x = x_coord + (( x - x_coord ) * cos( theta_radians )) - (( y - y_coord ) * sin( theta_radians ));
    y = y_coord + (( y - y_coord ) * cos( theta_radians )) + (( x - x_coord ) * sin( theta_radians ));
}

// operators
const Point operator+( const Point& lhs, const Point& rhs   )  { return Point( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z ); }
const Point operator-( const Point& lhs, const Point& rhs   )  { return Point( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z ); }
const Point operator*( const Point& lhs, const Point& rhs   )  { return Point( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z ); }
const Point operator/( const Point& lhs, const Point& rhs   )  { return Point( lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z ); }
const Point operator+( const Point& lhs, const GLfloat& rhs )  { return Point( lhs.x + rhs, lhs.y + rhs, lhs.z + rhs ); }
const Point operator-( const Point& lhs, const GLfloat& rhs )  { return Point( lhs.x - rhs, lhs.y - rhs, lhs.z - rhs ); }
const Point operator*( const Point& lhs, const GLfloat& rhs )  { return Point( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs ); }
const Point operator/( const Point& lhs, const GLfloat& rhs )  { return Point( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs ); }
const Point operator+( const GLfloat& lhs, const Point& rhs )  { return Point( lhs + rhs.x, lhs + rhs.y, lhs + rhs.z ); } 
const Point operator-( const GLfloat& lhs, const Point& rhs )  { return Point( lhs - rhs.x, lhs - rhs.y, lhs - rhs.z ); } 
const Point operator*( const GLfloat& lhs, const Point& rhs )  { return Point( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z ); } 
const Point operator/( const GLfloat& lhs, const Point& rhs )  { return Point( lhs / rhs.x, lhs / rhs.y, lhs / rhs.z ); }
const bool  operator==( const Point& lhs, const Point& rhs )   { return ( lhs.x == rhs.x && lhs.y == rhs.y ); }

} // namespace xmx
