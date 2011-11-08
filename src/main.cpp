#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;

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
}

//------------------------------------------------------------------------------
void gl_display_function()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glRenderMode( GL_RENDER );
    glutSwapBuffers();
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

    glutMainLoop();
    
    return 0;
}
