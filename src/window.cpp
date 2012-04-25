#include <boost/foreach.hpp>

#include <iostream>
#include <memory>
using std::shared_ptr;

#include <GL/glut.h>

#include "utils.h"
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

void Window::clickEvent( int click_x, int click_y, int button, int state )
{
    BOOST_FOREACH( shared_ptr< Control > control, controls )
    {
        if( control->isPointInside( click_x - x, click_y - y ))
            control->clickEvent( click_x - x, click_y - y, button, state );
    }
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

void Window::hoverEnterEvent( int x_, int y_ )
{
    Control::hoverEnterEvent( x_, y_ );

    bool over_a_control = false;
    BOOST_FOREACH( shared_ptr< Control > control, controls )
    {
        if( control->isPointInside( x_ - x, y_ - y ))
        {
            if( hovered_control && hovered_control != control )
                hovered_control->hoverLeaveEvent( x_ - x, y_ - y );

            hovered_control = control;
            over_a_control = true;
            control->hoverEnterEvent( x_ - x, y_ - y );
            break;
        }
    }

    if( !over_a_control && hovered_control )
    {
        hovered_control->hoverLeaveEvent( x_, y_ );
        hovered_control = shared_ptr< Control >();
    }

    glutPostRedisplay();
}

void Window::hoverLeaveEvent( int x_, int y_ )
{
    Control::hoverLeaveEvent( x_, y_ );

    if( hovered_control )
    {
        hovered_control->hoverLeaveEvent( x_, y_ );
        hovered_control = shared_ptr< Control >();
    }

    glutPostRedisplay();
}

} // namespace xmx
