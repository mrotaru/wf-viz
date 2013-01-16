#ifndef XMX_BEZIER_H
#define XMX_BEZIER_H
#include <string>

#include <GL/glu.h>

#include "globals.h"
#include "geometry/point.h"
#include "geometry/primitive.h"

namespace xmx {

class BezierCurve : virtual public Primitive
{
    public:
        // constructor parameters:
        // e1,e2: end points
        // c1,c2: control points
        BezierCurve( Point e1, Point c1, Point c2, Point e2 );
        BezierCurve( GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat );
        ~BezierCurve();

        // methods for Primitive
        void print();
        void print( std::string );
        void draw();
        void rotate( GLfloat );
        void rotate( GLfloat, GLfloat, GLfloat );
        void move( GLfloat, GLfloat );
        void scale( GLfloat, GLfloat );
        
        void drawBoundingBox();
        void calculateBoundingBox();
        void printBoundingBox();
        void drawToBuffer( GLfloat*, GLint& );
        void drawVertices( GLfloat* );
        void isLineOld();
        GLint numLines;
        GLint getNumLines();

        bool isLine;

        GLfloat getMaxX() { return maxX; }
        GLfloat getMinX() { return minX; }
        GLfloat getMaxY() { return maxY; }
        GLfloat getMinY() { return minY; }
        GLfloat distanceTo( GLfloat, GLfloat );

        GLfloat points[4][3]; // array holding the vertices
        static int count;     // total number of Bezier curves

        // bounding box
        GLfloat maxX, maxY, minX, minY;

        // sugar
        const static int END_PT_1  = 0;
        const static int CTRL_PT_1 = 1;
        const static int CTRL_PT_2 = 2;
        const static int END_PT_2  = 3;
        Point getP0() { return Point( points[0][0], points[0][1] ); }
        Point getP1() { return Point( points[1][0], points[1][1] ); }
        Point getP2() { return Point( points[2][0], points[2][1] ); }
        Point getP3() { return Point( points[3][0], points[3][1] ); }
        Point getLowerLeft()  { return Point( minX, minY ); }
        Point getUpperRight() { return Point( maxX, maxY ); }
};

} // namespace xmx
#endif /* XMX_BEZIER_H */
