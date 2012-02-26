#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#include <GL/glu.h>

#include "globals.h"
#include "utils.h"
#include "Point.h"
#include "BezierCurve.h"

namespace xmx {

int BezierCurve::count = 0;

BezierCurve::BezierCurve( Point e1, Point c1, Point c2, Point e2 )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for BezierCurve @ " << this << endl;
    // first end point
    points[ END_PT_1 ][0] = e1.x;
    points[ END_PT_1 ][1] = e1.y;
    points[ END_PT_1 ][2] = 0.0f; // 2D, so 'z' coord. is always 0

    // first control potint
    points[ CTRL_PT_1 ][0] = c1.x;
    points[ CTRL_PT_1 ][1] = c1.y;
    points[ CTRL_PT_1 ][2] = 0.0f;
    
    // second control potint
    points[ CTRL_PT_2 ][0] = c2.x;
    points[ CTRL_PT_2 ][1] = c2.y;
    points[ CTRL_PT_2 ][2] = 0.0f;

    // second end point
    points[ END_PT_2 ][0] = e2.x;
    points[ END_PT_2 ][1] = e2.y;
    points[ END_PT_2 ][2] = 0.0f; // 2D, so 'z' coord. is always 0
    
    BezierCurve::count ++;

    calculateBoundingBox();
    if( DEBUG_INIT_BEZIER_BOUNDING_BOX ) printBoundingBox();
}

BezierCurve::BezierCurve( 
        GLfloat e1_x, GLfloat e1_y,
        GLfloat c1_x, GLfloat c1_y,
        GLfloat c2_x, GLfloat c2_y,
        GLfloat e2_x, GLfloat e2_y )
{
    if( DEBUG_CTOR_DTOR ) cout<<"ctor for BezierCurve @ " << this << endl;
    // first end point
    points[ END_PT_1 ][0] = e1_x;
    points[ END_PT_1 ][1] = e1_y;
    points[ END_PT_1 ][2] = 0.0f; // 2D, so 'z' coord. is always 0

    // first control potint
    points[ CTRL_PT_1 ][0] = c1_x;
    points[ CTRL_PT_1 ][1] = c1_y;
    points[ CTRL_PT_1 ][2] = 0.0f;
    
    // second control potint
    points[ CTRL_PT_2 ][0] = c2_x;
    points[ CTRL_PT_2 ][1] = c2_y;
    points[ CTRL_PT_2 ][2] = 0.0f;

    // second end point
    points[ END_PT_2 ][0] = e2_x;
    points[ END_PT_2 ][1] = e2_y;
    points[ END_PT_2 ][2] = 0.0f; // 2D, so 'z' coord. is always 0

    BezierCurve::count ++;

    calculateBoundingBox();

    if( DEBUG_INIT_BEZIER_BOUNDING_BOX ) printBoundingBox();
}


BezierCurve::~BezierCurve()
{
    if( DEBUG_CTOR_DTOR ) cout<<"dtor for BezierCurve @ " << this << endl;
    BezierCurve::count --;
}

void BezierCurve::printBoundingBox()
{
    cout<< "maxX: " << maxX << endl;
    cout<< "maxY: " << maxY << endl;
    cout<< "minX: " << minX << endl;
    cout<< "minY: " << minY << endl;
}

GLint BezierCurve::getNumLines()
{
    if( isLine )
        return 1;
    return getMaxX() - getMinX();
}

void BezierCurve::draw()
{
    if( isLine )
    {
        glBegin( GL_LINES );
            glVertex3fv( points[ END_PT_1 ]);
            glVertex3fv( points[ END_PT_2 ]);
        glEnd();
        return;
    }

    GLfloat width = ( getMaxX() - getMinX() );
//    width = 100;

    glMap1f( GL_MAP1_VERTEX_3,
            0.0f,
            width,
            3,
            4, // number of control points
            &points[0][0]
           );

    glEnable( GL_MAP1_VERTEX_3 );

    glBegin( GL_LINE_STRIP );
		for( int i = 0; i < width; i++ )
			{
			// Evaluate the curve at this point
			glEvalCoord1f( (GLfloat)i ); 
            }
    glEnd();

    if( SHOW_BEZIER_POINTS )
    {
        glColor3f( dcol_CPoints.R, dcol_CPoints.G, dcol_CPoints.B );
        glPointSize( 5.0f );
        glBegin( GL_POINTS );
            glColor3f( dcol_EPoints.R, dcol_EPoints.G, dcol_EPoints.B );
            glVertex2fv( points[ END_PT_1  ] );
            glColor3f( dcol_CPoints.R, dcol_CPoints.G, dcol_CPoints.B );
            glVertex2fv( points[ CTRL_PT_1 ] );
            glVertex2fv( points[ CTRL_PT_2 ] );
            glColor3f( dcol_EPoints.R, dcol_EPoints.G, dcol_EPoints.B );
            glVertex2fv( points[ END_PT_2  ] );
        glEnd();
        
        if( SHOW_BEZIER_CONTROL_POINT_LINES )
        {
            glColor3f( dcol_ECLines.R, dcol_ECLines.G, dcol_ECLines.B );
            glBegin( GL_LINES );
                glVertex2fv( points[ END_PT_1 ] );
                glVertex2fv( points[ CTRL_PT_1] );
                glVertex2fv( points[ CTRL_PT_2 ] );
                glColor3f( dcol_EPoints.R, dcol_EPoints.G, dcol_EPoints.B );
                glVertex2fv( points[ END_PT_2  ] );
            glEnd();
        }
    glColor3f( dcol.R, dcol.G, dcol.B );
    }

    if( SHOW_BEZIER_BOUNDING_BOX ) drawBoundingBox();
}

// This function enables Shapes that contain only Bezier curves to be
// drawn as filled rectangles - therefore enabling them to have a fill color.
//
// How does it work ?
// ------------------
// Instead of rendering to the screen, this method uses the GL_FEEDBACK render
// mode to capture the vertices in a buffer, which is then copied to the
// `dest` buffer, starting at `start_pos`. Also, `start_pos` is updated to
// point to the last position in the buffer. The Shape's draw method takes care
// of rendering the resulting buffer as a filled polygon. 
void BezierCurve::drawToBuffer( GLfloat* dest, GLint& start_pos )
{
    if( isLine )
    {
        dest[ start_pos++ ] = points[ END_PT_1 ][0];
        dest[ start_pos++ ] = points[ END_PT_1 ][1];
        dest[ start_pos++ ] = points[ END_PT_2 ][0];
        dest[ start_pos++ ] = points[ END_PT_2 ][1];
        if( DEBUG_FEEDBACK_TOKENS )
            cout<<"written line bc to buffer; index = " << start_pos << endl;
        return;
    }

    GLfloat width = getMaxX() - getMinX();
//    width = 100;

    glMap1f( GL_MAP1_VERTEX_3,
            0.0f,
            width,
            3,
            4, // number of control points
            &points[0][0]
           );

    glEnable( GL_MAP1_VERTEX_3 );

    if( DEBUG_FEEDBACK_TOKENS )
    {
        char cr;
        cout<<"about to begin evaluating BezierCurve @ "<< this << " ( " << name << " ) " << endl;
        print();
        cout<<"number or lines in it: " << getNumLines() << endl;
        cin.get(cr);
    }

    int buff_size = (int)( width ) * 3 * 2 + 2 + width;
    GLfloat *buff = new GLfloat[ buff_size ];
    glFeedbackBuffer( buff_size, GL_3D, buff );
    
    glRenderMode( GL_FEEDBACK );

    glPassThrough( 1000.0f );
    glBegin( GL_LINE_STRIP );
		for( int i = 0; i < width; i++ )
			{
			// Evaluate the curve at this point
			glEvalCoord1f( (GLfloat)i ); 
            }
    glEnd();
    glPassThrough( 2000.0f );

    glFlush();
//    if( DEBUG_FEEDBACK_TOKENS ) printFeedbackBuffer( buff, buff_size );
    
    // go back through the buffer, drawing the curve as a filled polygon
//    glRenderMode( GL_RENDER );

//    glPolygonMode( GL_FRONT, GL_FILL );
//    glBegin( GL_POLYGON );
        for( int i=0; i < width-3; i++ )
        {
            if( DEBUG_FEEDBACK_TOKENS ) cout<<"copying line " << i + 1 << " of " << getNumLines() <<" of curve " << this << " : " << name << endl;
            GLint i7 = i * 7;
            // vertices
            GLint x1 = i7 + 3;
            GLint y1 = i7 + 4;
            GLint x2 = i7 + 6;
            GLint y2 = i7 + 7;
            if ( DEBUG_FEEDBACK_TOKENS ) cout<< "vertex1:   x: " << x1 << " > " << buff[ x1 ] << " y: " << y1 << " > " << buff[ y1 ] << endl;
//            glVertex2f( buff[ x1 ] , buff[ y1 ] );
            dest[ start_pos++ ] = buff[ x1 ]; 
            dest[ start_pos++ ] = buff[ y1 ]; 

            if ( DEBUG_FEEDBACK_TOKENS ) cout<< "vertex2:   x: " << x2 << " > " << buff[ x2 ] << " y: " << y2 << " > " << buff[ y2 ] << endl;
//            glVertex2f( buff[ x2 ] , buff[ y2 ] );
            dest[ start_pos++ ] = buff[ x2 ]; 
            dest[ start_pos++ ] = buff[ y2 ]; 
            if( DEBUG_FEEDBACK_TOKENS ) cout<<"start_pos: " << start_pos << endl;
        }
//    glEnd();
    

    if( DEBUG_FEEDBACK_TOKENS ) cout<< "attempting to delete buffer allocated for BezierCurve @ " << this << endl;
    delete[] buff;
    if( DEBUG_FEEDBACK_TOKENS ) cout<< "finished putting the lines of BezierCurve @ " << this << endl;
}

void BezierCurve::drawVertices( GLfloat *lastVertex )
{
    if( isLine )
    {
            glVertex3fv( lastVertex );
            glVertex3fv( points[ END_PT_1 ] );
            glVertex3fv( points[ END_PT_2 ] );

        if( DEBUG_FEEDBACK_TOKENS )
            cout<<"drew a BC which is a line" << endl;
        return;
    }

    GLfloat width = getMaxX() - getMinX();
//    width = 100;

    glMap1f( GL_MAP1_VERTEX_3,
            0.0f,
            width,
            3,
            4, // number of control points
            &points[0][0]
           );

    glEnable( GL_MAP1_VERTEX_3 );

    if( DEBUG_FEEDBACK_TOKENS )
    {
        char cr;
        cout<<"about to begin evaluating BezierCurve @ "<< this << " ( " << name << " ) " << endl;
        print();
        cout<<"number or lines in it: " << getNumLines() << endl;
        cin.get(cr);
    }

    int buff_size = (int)( width ) * 3 * 2 + 2 + width;
    GLfloat *buff = new GLfloat[ buff_size ];
    glFeedbackBuffer( buff_size, GL_3D, buff );
    
    glRenderMode( GL_FEEDBACK );

    glPassThrough( 1000.0f );
    glBegin( GL_LINE_STRIP );
		for( int i = 0; i < width; i++ )
			{
			// Evaluate the curve at this point
			glEvalCoord1f( (GLfloat)i ); 
            }
    glEnd();
    glPassThrough( 2000.0f );

    glFlush();
//    if( DEBUG_FEEDBACK_TOKENS ) printFeedbackBuffer( buff, buff_size );
    
    // go back through the buffer, drawing the curve as a filled polygon
    glRenderMode( GL_RENDER );

//    glPolygonMode( GL_FRONT, GL_FILL );
//    glBegin( GL_POLYGON );
        for( int i=0; i < width-3; i++ )
        {
            if( DEBUG_FEEDBACK_TOKENS ) cout<<"copying line " << i + 1 << " of " << getNumLines() <<" of curve " << this << " : " << name << endl;
            GLint i7 = i * 7;
            // vertices
            GLint x1 = i7 + 3;
            GLint y1 = i7 + 4;
            GLint x2 = i7 + 6;
            GLint y2 = i7 + 7;
            if ( DEBUG_FEEDBACK_TOKENS ) cout<< "vertex1:   x: " << x1 << " > " << buff[ x1 ] << " y: " << y1 << " > " << buff[ y1 ] << endl;
            glVertex2f( buff[ x1 ] , buff[ y1 ] );
//            dest[ start_pos++ ] = buff[ x1 ]; 
//            dest[ start_pos++ ] = buff[ y1 ]; 

            if ( DEBUG_FEEDBACK_TOKENS ) cout<< "vertex2:   x: " << x2 << " > " << buff[ x2 ] << " y: " << y2 << " > " << buff[ y2 ] << endl;
            glVertex2f( buff[ x2 ] , buff[ y2 ] );
//            dest[ start_pos++ ] = buff[ x2 ]; 
//            dest[ start_pos++ ] = buff[ y2 ]; 
//            if( DEBUG_FEEDBACK_TOKENS ) cout<<"start_pos: " << start_pos << endl;
        }
//    glEnd();
    

    if( DEBUG_FEEDBACK_TOKENS ) cout<< "attempting to delete buffer allocated for BezierCurve @ " << this << endl;
    delete[] buff;
    if( DEBUG_FEEDBACK_TOKENS ) cout<< "finished putting the lines of BezierCurve @ " << this << endl;
}
void BezierCurve::isLineOld()
{
    DEBUG_BEZIER_BOUNDING_BOX = true;
    print();
    calculateBoundingBox();
    DEBUG_BEZIER_BOUNDING_BOX = false;
}

void BezierCurve::print()
{
    cout<<"BezierCurve, at: "<<this<<endl;
    nice_cout( "EP1: x:", points[ END_PT_1  ][0]);
    nice_cout( "EP1: y:", points[ END_PT_1  ][1]);
    nice_cout( "CP1: x:", points[ CTRL_PT_1 ][0]);
    nice_cout( "CP1: y:", points[ CTRL_PT_1 ][1]);
    nice_cout( "CP2: x:", points[ CTRL_PT_2 ][0]);
    nice_cout( "CP2: y:", points[ CTRL_PT_2 ][1]);
    nice_cout( "EP2: x:", points[ END_PT_2  ][0]);
    nice_cout( "EP2: y:", points[ END_PT_2  ][1]);
    cout<<endl;
}

void BezierCurve::print( string str )
{
    cout<<str<<endl;
    print();
}

// solves the curve's equation for x, a,b,c,d
GLfloat solveFor( GLfloat x, GLfloat a, GLfloat b, GLfloat c, GLfloat d )
{
    return( a*x*x*x + b*x*x + c*x + d );
}

void BezierCurve::calculateBoundingBox()
{
    bool DBBB = DEBUG_BEZIER_BOUNDING_BOX;
    if( DBBB ) cout<< "entering calculateBoundingBox() for BezierCurve @ " << this << endl; 

    /*=====================================================================
    |                                                                     |
    |    P0 - first end point                                             |
    |    P1 - first control point                                         |
    |    P2 - second control point                                        |
    |    P3 - second end point                                            |
    |                                                                     |
    |    Cubic Bezier ecuation:                                           |
    |    f(x) = ax^3 + bx^2 + cx + d                                      |
    |      a = P3 - 3P2 + 3P1 -  P0                                       |
    |      b =      3P2 - 6P1 + 3P0                                       |
    |      c =            3P1 - 3P0                                       |
    |      d =                   P0                                       |
    |                                                                     |
    |    Derivative:                                                      |
    |    f'(x) = 3at^2 + 2bt + c                                          |
    |                                                                     |
    |    By solving f'(x) = 0, we get local extremes                      |
    |    replacing a-d in the derived ecuation:                           |
    |                                                                     |
    |    3( P3 - 3P2 + 3P1 - P0 )x^2 + 2( 3P2 - 6P1 + 3P0 )x + 3P1- 3P0   |
    |                                                                     |
    |    Determinant:                                                     |
    |    D = b^2 - 4ac                                                    |
    |                                                                     |
    \====================================================================*/

    Point P0 = getP0();
    Point P1 = getP1();
    Point P2 = getP2();
    Point P3 = getP3();

    // is this curve a straight line ?
    if( P0 == P1 && P2 == P3 )
    {
        if( DBBB )cout<<"this curve is a straight line"<<endl;
        isLine = true;
        // determine max and min x/y for end points
        maxX = P0.x > P3.x ? P0.x : P3.x;
        maxY = P0.y > P3.y ? P0.y : P3.y;
        minX = P0.x < P3.x ? P0.x : P3.x;
        minY = P0.y < P3.y ? P0.y : P3.y;
        return;
    }

    isLine = false;

    if( DBBB )cout<<"calculating coeficients..."<<endl;
    Point a = P3 - 3*P2 + 3*P1 - P0;     if( DBBB )a.print("a");
    Point b = 3*P2 - 6*P1 + 3*P0;        if( DBBB )b.print("b");
    Point c = 3*P1 - 3*P0;               if( DBBB )c.print("c");
    Point d = P0;                        if( DBBB )d.print("d");
    
    if( DBBB )cout<<"for the derived function: "<<endl;
    a = 3*a; if( DBBB )a.print("a");
    b = 2*b; if( DBBB )b.print("b");

    // Discriminant
    Point D = b*b - 4*( a * c );
    if( DBBB ) { cout<<"Discriminant: "; D.print("D"); }

    // solutions
    if( DBBB )cout<< "calcutating solutions..."<<endl;
    Point s1 = ( -1 * b + psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); if( DBBB )s1.print("s1");
    Point s2 = ( -1 * b - psqrt( b * b - 4 * a * c ))/ ( 2 * a  ); if( DBBB )s2.print("s2");

    // put solutions in an array
    GLfloat solutions[4]; 
    solutions[0] = s1.x;
    solutions[1] = s2.x;
    solutions[2] = s1.y; 
    solutions[3] = s2.y;

    // change a and b back to values for the original function, not the derivative
    a = a/3; if( DBBB )a.print("a");
    b = b/2; if( DBBB )b.print("b");

    // determine max and min x/y for end points
    maxX = P0.x > P3.x ? P0.x : P3.x;
    maxY = P0.y > P3.y ? P0.y : P3.y;
    minX = P0.x < P3.x ? P0.x : P3.x;
    minY = P0.y < P3.y ? P0.y : P3.y;

    // calculate the Bezier's equation for each solution
    for( int i=0; i<=3; i++ )
        if( solutions[i] < 1 && solutions[i] > 0 ) // Beziers are only defined for [0,1]
        {
            GLfloat x = solveFor( solutions[i], a.x, b.x, c.x, d.x ); if( DBBB )cout<< "s-x" <<i+1<<": "<< x << endl;
            GLfloat y = solveFor( solutions[i], a.y, b.y, c.y, d.y ); if( DBBB )cout<< "s-y" <<i+1<<": "<< y << endl;

            // new min or max ?
            maxX = x > maxX ? x : maxX;
            maxY = y > maxY ? y : maxY;
            minX = x < minX ? x : minX;
            minY = y < minY ? y : minY;
        }

    if( DEBUG_BEZIER_BOUNDING_BOX )
    {
        char cr;
        cout<<"computed bounding box for BezierCurve @ "<< this << " ( " << name << " ) " << endl;
        cin.get(cr);
    }
}

void BezierCurve::drawBoundingBox()
{
    glColor3f( dcol_BezierBB.R, dcol_BezierBB.G, dcol_BezierBB.B );
    glBegin( GL_LINES );
        glVertex2f( minX, minY );
        glVertex2f( maxX, minY );

        glVertex2f( maxX, minY );
        glVertex2f( maxX, maxY );
        
        glVertex2f( maxX, maxY );
        glVertex2f( minX, maxY );
        
        glVertex2f( minX, maxY );
        glVertex2f( minX, minY );
    glEnd();
    glColor3f( dcol.R, dcol.G, dcol.B );
}


void BezierCurve::rotate( GLfloat theta, GLfloat x_rel, GLfloat y_rel )
{
    if( DEBUG_TRANSF_ROTATE ) cout<<endl<<"rotating BezierCurve @ "<< this << endl;

    Point P;
    // first end point
    P = getP0();
    P.rotate( theta, x_rel, y_rel );
    points[ END_PT_1 ][0] = P.x;
    points[ END_PT_1 ][1] = P.y;
    points[ END_PT_1 ][2] = 0.0f;

    // first control potint
    P = getP1();
    P.rotate( theta, x_rel, y_rel );
    points[ CTRL_PT_1 ][0] = P.x;
    points[ CTRL_PT_1 ][1] = P.y;
    points[ CTRL_PT_1 ][2] = 0.0f;
    
    // second control potint
    P = getP2();
    P.rotate( theta, x_rel, y_rel );
    points[ CTRL_PT_2 ][0] = P.x;
    points[ CTRL_PT_2 ][1] = P.y;
    points[ CTRL_PT_2 ][2] = 0.0f;

    // second end point
    P = getP3();
    P.rotate( theta, x_rel, y_rel );
    points[ END_PT_2 ][0] = P.x;
    points[ END_PT_2 ][1] = P.y;
    points[ END_PT_2 ][2] = 0.0f;

    calculateBoundingBox();
    if( DEBUG_TRANSF_ROTATE ) { cout<<"after rotation: "; print(); char c; cin.get(c); }

}

void BezierCurve::rotate( GLfloat theta )
{
    GLfloat center_x = ( (getMaxX() - getMinX())/2 + getMinX() );
    GLfloat center_y = ( (getMaxY() - getMinY())/2 + getMinY() );
    rotate( theta, center_x, center_y );
}


GLfloat BezierCurve::distanceTo( GLfloat, GLfloat )
{
    return -1;
}

void BezierCurve::move( GLfloat x_dir, GLfloat y_dir )
{
    if( DEBUG_TRANSF_MOVE ) cout << "moving BezierCurve @ " << this << endl;
    for( int i=0; i<=3; i++ )
    {
        points[i][0] += x_dir;
        points[i][1] += y_dir;
    }

    if( DEBUG_TRANSF_MOVE )
    {
       cout << "P0: " << endl;
       getP0().print();
    }
    calculateBoundingBox();
}

void BezierCurve::scale( GLfloat, GLfloat )
{
    return /* something */;
}

} // namespace xmx
