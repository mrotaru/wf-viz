#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/regex.hpp>

namespace xmx {

typedef unsigned int uint;

struct Color
{ 
    float R,G,B;
    Color( float r, float g, float b ) : R(r), G(g), B(b) {}
    Color() : R( 0.5f ), G( 0.5f ), B( 0.5f ) {}
    void print();
};

template < typename T > T getFromRegex( string , const boost::regex& );
template < typename T > inline std::string to_string( const T& p );
void printText( int nX, int nY, string text );
void printBigText( int nX, int nY, string text );
void setColor( const Color& );
void setColor( const Color* );
void setColor( shared_ptr< Color > );
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLfloat dot( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLfloat degrees_to_radians( GLfloat );
shared_ptr< Color > getColorAt( float );

typedef void (*fptr_click_event)();
typedef void (*fptr_checkbox_event)(bool);

} // namespace xmx
#endif /* UTILS_H */
