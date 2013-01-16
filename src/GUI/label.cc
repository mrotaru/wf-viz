#include <GL/glu.h>
#include <GL/glut.h>

#include "utils.h"
#include "GUI/gui_utils.h"
#include "GUI/window.h"
#include "GUI/label.h"

namespace xmx
{

void Label::draw()
{
    Control::draw();
    int parent_x = parent->getX();
    int parent_y = parent->getY();

    // draw text
    setColor( text_color );
    printText( parent_x + x + 5, toGl( parent_y + y + 13 ), text );
}

} // namespace xmx
