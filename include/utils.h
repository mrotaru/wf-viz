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
    GLfloat R,G,B;
    Color( GLfloat r, GLfloat g, GLfloat b ) : R(r), G(g), B(b) {}
    Color() : R( 0.5f ), G( 0.5f ), B( 0.5f ) {}
};

template < typename T > T getFromRegex( string , const boost::regex& );
template < typename T > inline std::string to_string( const T& p );
void printText( int nX, int nY, string text );
void printBigText( int nX, int nY, string text );
void setColor( const Color& );
void setColor( const Color* );
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLfloat dot( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLfloat degrees_to_radians( GLfloat );

typedef void (*fptr_click_event)();
typedef void (*fptr_checkbox_event)(bool);

extern map< string, string > ISO3_codes;

} // namespace xmx
#endif /* UTILS_H */
