#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <iostream>
using namespace std;

#include "gui_surface.h"
#include "control.h"

namespace xmx
{

class Label: public Control
{
public:
    Label(): Control( 10, 10, 100, 50, "label", false ), text("label") {}
    Label( std::string text_ ): Control( 10, 10, 100, 50, "label", false ), text( text_ ) {}
    Label( int width_, int height_, std::string text_ ):
        Control( 0, 0, width_, height_, "label", false ), text( text_ )
        { setTextColor( &BLACK ); }
    void draw();

    void clickEvent( int, int ) {}
    void hoverEnterEvent( int, int ) {}
    void hoverLeaveEvent( int, int ) {}

    std::string getText()                         { return text; }
    void setText( std::string _text )             { text = _text; }

protected:
    string text;
};

} // namespace xmx
#endif /* LABEL_H */
