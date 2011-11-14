#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

#ifdef _MSC_VER
#include <windows.h>
#endif

#include <GL/glu.h>
#include <GL/glut.h>

namespace xmx {

//-----------------------------------------------------------------------------
void printText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glRasterPos2i( nX, nY );

    for( p=pszText, lines=0; *p; p++ )
    {
        if( *p == '\n' )
        {
            lines++;
            glRasterPos2i( nX, nY-(lines*18) );
        }

        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12,  *p );
    }
}

//-----------------------------------------------------------------------------
void printBigText( int nX, int nY, string text )
{
    int lines;
    const char *pszText = text.c_str();
    const char *p;

    glRasterPos2i( nX, nY );

    for( p=pszText, lines=0; *p; p++ )
    {
        if( *p == '\n' )
        {
            lines++;
            glRasterPos2i( nX, nY-(lines*18) );
        }

        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18,  *p );
    }
}

//-----------------------------------------------------------------------------
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 )
{
    return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

} // namespace xmx
