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
    glLineWidth( border_width );
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


bool Control::isPointInside( int x_, int y_ )
{ 
//    std::cout << "checking if " << to_string(x_) << ", " << to_string(y) <<" is inside " << name << endl;
//    std::cout <<" ( " << to_string(x) << ", " << to_string(y) << endl;
//    std::cout <<" ( " << to_string(width) << ", " << to_string(height) << endl;
    return(  x_ >= x && x_ <= x + width 
          && y_ >= y && y_ <= y + height );
}

} /* namespace xmx */
