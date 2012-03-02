#ifndef UTILS_H
#define UTILS_H

#include <list>
#include <string>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace xmx {

struct Color
{ 
    GLfloat R,G,B;
    Color( GLfloat r, GLfloat g, GLfloat b ) : R(r), G(g), B(b) {}
    Color() : R( 0.5f ), G( 0.5f ), B( 0.5f ) {}
};

template < typename T > T getFromRegex( string , const boost::regex& );
void printText( int nX, int nY, string text );
void printBigText( int nX, int nY, string text );
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLfloat degrees_to_radians( GLfloat );

} // namespace xmx
#endif /* UTILS_H */
