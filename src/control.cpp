#include <iostream>
using namespace std;

#include <GL/glu.h>
#include <GL/glut.h>

#include "utils.h"
#include "window.h"
#include "gui_utils.h"
#include "control.h"
#include "globals.h"

namespace xmx 
{

void Control::draw()
{
    // draw background
    useBackgroundColor();
    int parent_x = parent->getX();
    int parent_y = parent->getY();

    glBegin(GL_QUADS);
        glVertex2f( parent_x + x, toGl( parent_y + y ) );
        glVertex2f( parent_x + x + width, toGl( parent_y + y ) );
        glVertex2f( parent_x + x + width, toGl( parent_y + y + height ) );
        glVertex2f( parent_x + x, toGl( parent_y + y + height ) );
    glEnd();

    // draw border
    useBorderColor();
    glLineWidth( 1.0f );
    if( draw_borders )
    {
        glBegin(GL_LINE_LOOP);
            glVertex2f( parent_x + x, toGl( parent_y + y ) );
            glVertex2f( parent_x + x + width, toGl( parent_y + y ) );
            glVertex2f( parent_x + x + width, toGl( parent_y + y + height ) );
            glVertex2f( parent_x + x, toGl( parent_y + y + height ) );
        glEnd();
    }

    // set color to the default
    setColor( dcol_Clear );
}

} /* namespace xmx */
