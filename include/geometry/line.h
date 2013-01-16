#ifndef XMX_LINE_H
#define XMX_LINE_H

#include <string>
#include <GL/glu.h>
#include "geometry/primitive.h"

namespace xmx {

class Point;

class Line : virtual public Primitive
{
public:
    // constructors/destructors
    Line( int, int, int, int );
    Line( GLfloat, GLfloat, GLfloat, GLfloat );
    Line( const Point&, const Point& );
    ~Line();
    
    std::string name;

    // methods
    void move( GLfloat, GLfloat );
    void scale( GLfloat, GLfloat );
    void rotate( GLfloat );
    void rotate( GLfloat, GLfloat, GLfloat );
    void draw();
    void print();
    void print( std::string );
    GLfloat distanceTo( GLfloat, GLfloat );
    virtual GLfloat getMaxX();
    virtual GLfloat getMinX();
    virtual GLfloat getMaxY();
    virtual GLfloat getMinY();
    void rotate2( GLfloat );
    GLfloat getLength();

    // getters/setters
    GLfloat getX1() const { return x1; }
    GLfloat getY1() const { return y1; }
    GLfloat getX2() const { return x2; }
    GLfloat getY2() const { return y2; }
    void setX1 ( GLfloat _x1 ) { x1 = _x1; }
    void setY1 ( GLfloat _y1 ) { y1 = _y1; }
    void setX2 ( GLfloat _x2 ) { x2 = _x2; }
    void setY2 ( GLfloat _y2 ) { y2 = _y2; }

private:
    GLfloat x1,y1,x2,y2;
};

} // namespace xmx
#endif /* XMX_LINE_H */
