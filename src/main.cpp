#include <iostream>
#include <fstream>
using namespace std;

#ifdef _MSC_VER
#include <windows.h>
#endif

#include "utils.h"
using namespace xmx;

#include <GL/glu.h>
#include <GL/glut.h>

string VERSION    = "?";
string BUILD_ID   = "?";
string BUILD_TIME = "?";
string build_info = "";

//------------------------------------------------------------------------------
void gl_init()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    // initialize viewing values
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, GLUT_WINDOW_HEIGHT, 0, GLUT_WINDOW_WIDTH );
}

//------------------------------------------------------------------------------
void app_init()
{
    // read version number and build info
    ifstream ver_file( "VERSION", ifstream::in );
    getline( ver_file, VERSION );
    VERSION = "World Statistics, " + VERSION;
    getline( ver_file, BUILD_ID );
    getline( ver_file, BUILD_TIME );
    build_info = "Build info: " + BUILD_ID + " @ " + BUILD_TIME;
}

//------------------------------------------------------------------------------
void gl_display_function()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glRenderMode( GL_RENDER );

    glColor3ub( 60, 60, 60 );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 18, VERSION );
    printText( 10, glutGet( GLUT_WINDOW_HEIGHT ) - 32, build_info );

    glutSwapBuffers();
}

//------------------------------------------------------------------------------
void gl_reshape( int nWidht, int nHeight )
{
    glViewport( 0, 0, (GLsizei)nWidht, (GLsizei)nHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, nWidht, 0, nHeight);
    glutPostRedisplay();
}

//------------------------------------------------------------------------------
void gl_keyboard_function( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 27:
            exit(0);
            break;
    }
}

//------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE );
     
    // window
    glutInitWindowSize( 800, 600 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "World Factbook" );

    gl_init();  // OpenGL-specific initializations
    app_init(); // application-specific initializations

    // callbacks
    glutDisplayFunc( gl_display_function );
    glutKeyboardFunc( gl_keyboard_function );
    glutReshapeFunc( gl_reshape );

    glutMainLoop();
    
    return 0;
}
