#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>
#include <iostream>
using namespace std;

#include "gui_utils.h"
#include "control.h"

namespace xmx {

class CheckBox: public Control
{
public:
    CheckBox ( int width_, int height_, std::string text_ ):
        Control( 0, 0,  width_, height_, "checkbox", false ), text( text_ ),
        checked( false ), pressed( false )
        {}

    void draw();

    // events
    //--------------------------------------------------------------------------
    void clickEvent( int x_, int y_, int button, int state );
    void dragEvent ( int x_, int y_ ) {}
    void hoverEnterEvent( int, int ) { hovered = true; };
    void hoverLeaveEvent( int, int ) { hovered = false; };

    // property get/set
    //--------------------------------------------------------------------------
    std::string getText()                         { return text; }
    void setText( std::string _text )             { text = _text; }

protected:
    string text;
    bool checked;
    bool pressed;

    static Color* CHECKBOX_BORDER_COLOR;
    static Color* CHECKBOX_CHECKED_COLOR;
    static Color* CHECKBOX_HOVER_COLOR;
    static Color* CHECKBOX_UNCHECKED_COLOR;
};

} // namespace xmx
#endif /* CHECKBOX_H */
