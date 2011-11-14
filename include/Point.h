#ifndef POINT_H
#define POINT_H

#include <string>
#include <GL/glu.h>

namespace xmx {

class Point
{
public:
    // constructors / destructors
    Point(): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    Point( GLfloat _x, GLfloat _y, GLfloat _z ): x(_x), y(_y), z(_z) {}
    Point( GLfloat _x, GLfloat _y ): x(_x), y(_y), z( 0 ) {}
    
    // operators
    friend const Point operator+( const Point& lhs, const Point& rhs   );
    friend const Point operator-( const Point& lhs, const Point& rhs   );
    friend const Point operator*( const Point& lhs, const Point& rhs   );
    friend const Point operator/( const Point& lhs, const Point& rhs   );
    friend const Point operator+( const Point& lhs, const GLfloat& rhs );
    friend const Point operator-( const Point& lhs, const GLfloat& rhs );
    friend const Point operator*( const Point& lhs, const GLfloat& rhs );
    friend const Point operator/( const Point& lhs, const GLfloat& rhs );
    friend const Point operator+( const GLfloat& lhs, const Point& rhs );
    friend const Point operator-( const GLfloat& lhs, const Point& rhs );
    friend const Point operator*( const GLfloat& lhs, const Point& rhs );
    friend const Point operator/( const GLfloat& lhs, const Point& rhs );
    friend const bool  operator==( const Point& lhs, const Point& rhs );

    // methods
    void print();
    void print( std::string );
    void rotate( const GLfloat& theta );
    void rotate( const GLfloat& theta, const Point& P );
    void rotate( const GLfloat& theta, const GLfloat& x_coord, const GLfloat& y_coord );

    // getters/setters
    GLfloat getX() const      { return x; }
    GLfloat getY() const      { return y; }
    GLfloat getZ() const      { return z; }
    void setX( GLfloat _x ) { x = _x; }
    void setY( GLfloat _y ) { y = _y; }
    void setZ( GLfloat _z ) { x = _z; }

private:
    GLfloat x,y,z;
};

Point psqrt( const Point& );
void nice_cout( std::string, GLfloat );

} // namespace xmx
#endif /* POINT_H */
