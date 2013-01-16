#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <GL/glu.h>
#include <boost/shared_ptr.hpp>

#include "utils.h"
#include "globals.h"
#include "geometry/shape.h"

namespace xmx {

class Group
{
public:
    std::string name;
    Color color;
    GLfloat lineWidth;

    Group();
    ~Group();


    list< boost::shared_ptr<Shape    > > shapes;
    int getNumberOfShapes()      { return shapes.size();     };

    void scale ( GLfloat, GLfloat );
    void rotate( GLfloat );
//    void rotate( GLfloat, GLfloat, GLfloat );
    void move  ( GLfloat, GLfloat );
    void draw();
    void print();
    GLfloat getMaxX();
    GLfloat getMinX();
    GLfloat getMaxY();
    GLfloat getMinY();

    void addShape( Shape& shape );
    void loadFromPovFile( std::string );
    void setColor( GLfloat, GLfloat, GLfloat );
    void setColor( Color& );
};

} // namespace xmx
#endif /* GROUP_H */
