#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include <GL/glu.h>

namespace xmx 
{

class Primitive
{
public:
    std::string name;

    virtual void scale ( GLfloat, GLfloat ) = 0;
    virtual void rotate( GLfloat ) = 0;               // around the center of the bounding box
    virtual void rotate( GLfloat, GLfloat, GLfloat ) = 0; // around specified point
    virtual void move  ( GLfloat, GLfloat ) = 0;
    virtual void draw() = 0;
    virtual GLfloat distanceTo( GLfloat, GLfloat ) = 0;
    virtual void print () = 0;
    virtual void print ( std::string ) = 0;
    
    virtual GLfloat getMaxX( )=0;
    virtual GLfloat getMinX( )=0;
    virtual GLfloat getMaxY( )=0;
    virtual GLfloat getMinY( )=0;

};

} // namespace xmx
#endif /* PRIMITIVE_H */
