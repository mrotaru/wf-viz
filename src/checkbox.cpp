#include "gui_utils.h"
#include "window.h"
#include "checkbox.h"

namespace xmx {

Color* CheckBox::CHECKBOX_BORDER_COLOR = const_cast< Color* >( &DIM_GREY );
Color* CheckBox::CHECKBOX_CHECKED_COLOR = const_cast< Color* >( &BLACK );
Color* CheckBox::CHECKBOX_HOVER_COLOR = const_cast< Color* >( &LIGHT_GREY );
Color* CheckBox::CHECKBOX_UNCHECKED_COLOR = const_cast< Color* >( &GAINSBORO );

void CheckBox::draw()
{
    setBackgroundColor( hovered ? CHECKBOX_HOVER_COLOR: &control_bg );
    Control::draw();

    int parent_x = parent->getX();
    int parent_y = parent->getY();

    // draw text
    setColor( BLACK );
    printText( parent_x + x + 20, toGl( parent_y + y + 13 ), text );

    int b_x1 = 1;
    int b_y1 = 4;
    int b_x2 = 12;
    int b_y2 = 3;

    // draw checkbox border
    setColor( CHECKBOX_BORDER_COLOR );
    glLineWidth( 1 );
    glBegin(GL_LINE_LOOP);
        glVertex2f( parent_x + x + b_x1, toGl( parent_y + y + b_y1 ) );
        glVertex2f( parent_x + x + b_x2, toGl( parent_y + y + b_y1 ) );
        glVertex2f( parent_x + x + b_x2, toGl( parent_y + y + height - b_y2 ) );
        glVertex2f( parent_x + x + b_x1, toGl( parent_y + y + height - b_y2 ) );
    glEnd();

    // draw checkbox square
    setColor( checked ? CHECKBOX_CHECKED_COLOR: CHECKBOX_UNCHECKED_COLOR );
    glBegin(GL_QUADS);
        glVertex2f( parent_x + x + b_x1 + 1, toGl( parent_y + y + b_y1 + 1 ) );
        glVertex2f( parent_x + x + b_x2 - 2, toGl( parent_y + y + b_y1 + 1 ) );
        glVertex2f( parent_x + x + b_x2 - 2, toGl( parent_y + y + height - b_y2 - 2 ) );
        glVertex2f( parent_x + x + b_x1 + 1, toGl( parent_y + y + height - b_y2 - 2 ) );
    glEnd();
}

void CheckBox::clickEvent( int x_, int y_, int button, int state )
{
    if( button == 0 && state == 0 ) // left click
        checked = !checked;
}

} // namespace xmx
