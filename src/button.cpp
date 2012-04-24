#include <GL/glut.h>

#include "window.h"
#include "button.h"
#include "gui_utils.h"

namespace xmx
{

void Button::clickEvent( int x_, int y_, int button, int state )
{ 
    pressed = state ? false : true ;
    glutPostRedisplay();

    if( onClick )
        (*onClick)();
}

void Button::draw()
{
    if(!pressed)
        setBackgroundColor( hovered ? &LIGHTER_GREY : &GAINSBORO );
    else setBackgroundColor( &LIGHT_GREY );

    Control::draw();
    int parent_x = parent->getX();
    int parent_y = parent->getY();

    // draw another border
    setColor( &DARK_GREY );
    glLineWidth( 1 );
    if( draw_borders )
    {
        glBegin(GL_LINE_LOOP);
            glVertex2f( parent_x + x - 1, toGl( parent_y + y - 1 ) );
            glVertex2f( parent_x + x + width, toGl( parent_y + y ) );
            glVertex2f( parent_x + x + width, toGl( parent_y + y + height ) );
            glVertex2f( parent_x + x, toGl( parent_y + y + height ) );
        glEnd();
    }

    // for 3d effect
    setColor( &control_bg );
    glLineWidth( 1 );
    glBegin(GL_LINE_STRIP);
        glVertex2f( parent_x + x + width - 1, toGl( parent_y + y ) );
        glVertex2f( parent_x + x + width - 1, toGl( parent_y + y + height - 1 ) );
        glVertex2f( parent_x + x, toGl( parent_y + y + height -1 ) );
    glEnd();

    setColor( &LIGHT_GREY );
    glBegin(GL_POLYGON);
        glVertex2f( parent_x + x + 2, toGl( parent_y + y + ( height - 4 ) ) );
        glVertex2f( parent_x + x + width - 2, toGl( parent_y + y + ( height - 4 ) ) );
        glVertex2f( parent_x + x + width - 2, toGl( parent_y + y + height - 2 ) );
        glVertex2f( parent_x + x + 2, toGl( parent_y + y + height - 2 ) );
    glEnd();

    // draw text
    setColor( text_color );
    printText( parent_x + x + 5, toGl( parent_y + y + 13 ), text );
}

} // namespace xmx
