#include <boost/foreach.hpp>

#include <iostream>
#include <memory>
using std::shared_ptr;

#include "window.h"
#include "control.h"

namespace xmx
{

void Window::addControl( shared_ptr<Control> ctrl, int x_, int y_ )
{
    ctrl->setX( x_ );
    ctrl->setY( y_ );
    ctrl->setParent( this );
    controls.push_back( ctrl );
}

void Window::draw()
{
    // draw background
    useBackgroundColor();
    glBegin(GL_QUADS);
        glVertex2f( x, toGl( y ) );
        glVertex2f( x + width, toGl( y ) );
        glVertex2f( x + width, toGl( y + height ) );
        glVertex2f( x, toGl( y + height ) );
    glEnd();

    // draw border
    useBorderColor();
    glLineWidth( 1.0f );
    if( draw_borders )
    {
        glBegin(GL_LINE_LOOP);
            glVertex2f( x, toGl( y ) );
            glVertex2f( x + width, toGl( y ) );
            glVertex2f( x + width, toGl( y + height ) );
            glVertex2f( x, toGl( y + height ) );
        glEnd();
    }

    BOOST_FOREACH( shared_ptr< Control > sp_control, controls )
    {
        sp_control->draw();
    }

    // draw title bar
    titleBar->draw();
}

} // namespace xmx
