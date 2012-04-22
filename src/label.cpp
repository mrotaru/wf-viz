#ifndef LABEL_CPP
#define LABEL_CPP

#include <GL/glu.h>
#include <GL/glut.h>

#include "label.h"

namespace xmx
{

void Label::draw()
{
    // draw border
    glBegin(GL_LINE_LOOP);
        glVertex2f( x, y );
        glVertex2f( x + width, y );
        glVertex2f( x + width, y + height );
        glVertex2f( x, y + height);
    glEnd();

    // draw background
    glBegin(GL_QUADS);
        glVertex2f( x, y );
        glVertex2f( x + width, y );
        glVertex2f( x + width, y + height );
        glVertex2f( x, y + height);
    glEnd();

    // draw text
}

} // namespace xmx
#endif /* LABEL_CPP */
