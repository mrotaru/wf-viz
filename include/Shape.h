#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <list>
using namespace std;

#include <GL/glu.h>
#include <boost/shared_ptr.hpp>

#include "utils.h"
#include "Point.h"
#include "Primitive.h"
#include "BezierCurve.h"

namespace xmx {

class Shape
{
public:
    Shape();
    ~Shape();

    list< boost::shared_ptr<Primitive> > primitives;

    bool isSelected;
    bool isBezier;
    string name;
    static int count;
    Color color;
    GLfloat lineWidth;

    void addLine( GLfloat, GLfloat, GLfloat, GLfloat );
    void addBezierCurve( GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
    void addBezierCurve( Point&, Point&, Point&, Point& );
    void addBezierCurve( BezierCurve* );
    void addBezierCurve( boost::shared_ptr< BezierCurve > );

    void move( GLfloat, GLfloat );
    void scale( GLfloat, GLfloat );
    void rotate( GLfloat );
    void rotate( GLfloat, GLfloat, GLfloat );
    void print();
    void print( std::string );
    void draw();
    GLfloat distanceTo( GLfloat, GLfloat );
    
    GLfloat getMaxX();
    GLfloat getMinX();
    GLfloat getMaxY();
    GLfloat getMinY();

    GLint getNumLines();

};

} // namespace xmx
#endif /* SHAPE_H */
