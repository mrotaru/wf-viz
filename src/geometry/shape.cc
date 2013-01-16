#include <iostream>
#include <fstream>
#include <typeinfo>
using namespace std;

#include <GL/glu.h>

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/foreach.hpp>

#include "globals.h"
#include "geometry/point.h"
#include "geometry/primitive.h"
#include "geometry/shape.h"
#include "geometry/bezier_curve.h"
#include "geometry/line.h"

namespace xmx {

int Shape::count = 0;

Shape::Shape() : color( dcol ), lineWidth( 1.0f )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for Shape @ " << this << endl;
    isBezier = false;

    Shape::count++;
}

Shape::~Shape()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for Shape @ " << this << " ( " << name <<" )" << endl;
    Shape::count--;
}

void Shape::addBezierCurve(
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( e1_x, e1_y, c1_x, c1_y, c2_x, c2_y, e2_x, e2_y ));

    primitives.push_back( mptr );
}

void Shape::addBezierCurve(
        Point& P0, Point& P1, Point& P2, Point& P3
        )
{
    boost::shared_ptr<BezierCurve> mptr
        ( new BezierCurve( P0, P1, P2, P3 ));

    primitives.push_back( mptr );
}

void Shape::addBezierCurve( BezierCurve* ptr_bc )
{
    boost::shared_ptr< BezierCurve > mptr( ptr_bc );
    primitives.push_back( mptr );
}

void Shape::addBezierCurve( boost::shared_ptr< BezierCurve > sptr )
{
    primitives.push_back( sptr );
}

void Shape::addLine( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    boost::shared_ptr< Line > mptr
        ( new Line( x1, y1, x2, y2 ));

    primitives.push_back( mptr );
    isBezier = false;
}


void Shape::scale( GLfloat x_dir, GLfloat y_dir )
{
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive-> scale( x_dir, y_dir );
    }
}

void Shape::rotate( GLfloat theta )
{
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive-> rotate( theta );
    }
}

void Shape::rotate( GLfloat theta, GLfloat x_coord, GLfloat y_coord )
{
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive-> rotate( theta, x_coord, y_coord );
    }
}

void Shape::move( GLfloat x_dir, GLfloat y_dir )
{
    if ( DEBUG_TRANSF_MOVE ) cout << "moving Shape @ " << this << endl;
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
    {
        pPrimitive->move( x_dir, y_dir );
    }
}

void Shape::print()
{
    cout << "printing Shape @ " << this;
    if( name != "" ) cout <<", '" << name << "'";
    cout << endl;
    cout << "number of primitives: " << primitives.size() << endl;
}


void Shape::draw()
{

    glColor3f( color.R, color.G, color.B );
    glLineWidth( lineWidth );

    if( !isBezier )
    {
        GLint total_lines = 0;

        // how many lines in this shape ? ( the sum of lines for all BC's contained )
        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"counting lines composing Shape @ " << this << " ( " << name << " )" << endl; 
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            BezierCurve* bc = dynamic_cast< BezierCurve* > ( pPrimitive.get() );
            if( bc != NULL )
            {
                if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"    lines composing BezierCurve @ " << bc << " ( " << bc->name << " ) ... "; 
                GLint bc_lines = bc -> getNumLines();
                total_lines += bc_lines;
                if( DEBUG_SHAPE_BEZIER_DRAW ) cout<< bc_lines << " ( new total: " << total_lines << " )" << endl;
            }
        }
        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"done counting lines composing Shape @ " << this << " ( " << name << " ) : " << total_lines << endl; 

        GLint index = 0;
        GLint buff_size = total_lines * sizeof(GLfloat) * 4;
        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<< "allocating memory for an array of " << buff_size <<" GLfloat's" << endl;
        GLfloat *shapes_lines = new GLfloat[ buff_size ]; 
        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<< "size of buffer: " << sizeof(shapes_lines) << endl;

        if( DEBUG_SHAPE_BEZIER_DRAW ) cout<<"starting to put lines in buffer for Shape @ " << this << " ( " << name << " ) " << endl; 

        // gather the lines which form all Bezier curves in this Shape
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            BezierCurve* bc = dynamic_cast< BezierCurve* > ( pPrimitive.get() );
            cout<<"-- LOADING LINES --" << endl;
            bc -> print();
            if( bc != NULL )
            {
                bc->drawToBuffer( shapes_lines, index );
                if( DEBUG_FEEDBACK_TOKENS ) cout <<"BezierCurve @ " << bc <<" finished putting it's lines in the buffer " << endl;

            }
        }
    
        if( DEBUG_FEEDBACK_TOKENS ) cout <<"finished putting lines into the buffer" << endl;

        if( DEBUG_SHAPE_BEZIER_DRAW )
        {
            cout<<"finished putting lines in buffer for Shape @ " << this << " ( " << name << " ) " << endl; 
            cout<<"index: " << index << endl;
        }

        // draw a polygon from the vertices in the shapes_lines

        delete[] shapes_lines;

    }
    else // contains other types of primitives ( not only beziers )
    {
        BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        {
            pPrimitive->draw();
        }
    }
    
    glColor3f( dcol.R, dcol.G, dcol.B );
    glLineWidth( lineWidth );
}

GLfloat Shape::getMaxX()
{
    GLfloat maxX = primitives.front() -> getMaxX();
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        if( pPrimitive -> getMaxX() > maxX ) maxX = pPrimitive -> getMaxX();
    return maxX;
}

GLfloat Shape::getMinX()
{
    GLfloat minX = primitives.front() -> getMinX();
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        if( pPrimitive -> getMinX() < minX ) minX = pPrimitive -> getMinX();
    return minX;
}

GLfloat Shape::getMaxY()
{
    GLfloat maxY = primitives.front() -> getMaxY();
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        if( pPrimitive -> getMaxY() > maxY ) maxY = pPrimitive -> getMaxY();
    return maxY;
}

GLfloat Shape::getMinY()
{
    GLfloat minY = primitives.front() -> getMinY();
    BOOST_FOREACH( boost::shared_ptr< Primitive > pPrimitive, primitives )
        if( pPrimitive -> getMinY() < minY ) minY = pPrimitive -> getMinY();
    return minY;
}

GLfloat Shape::distanceTo( GLfloat x, GLfloat y )
{
    return -1001;
}

} // namespace xmx
