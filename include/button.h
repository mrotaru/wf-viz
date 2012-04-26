#ifndef BUTTON_H
#define BUTTON_H

#include "gui_utils.h"
#include "control.h"

namespace xmx {

class Button: public Control
{
public:
    Button( int width_, int height_, std::string text_ ):
        Control( 0, 0, width_, height_, "button", true ), text( text_ ),
        onClick( 0 ), pressed( false )
        {   setTextColor( &BLACK );
            setBackgroundColor( &GAINSBORO );
            setBorderColor( &LIGHT_GREY );
            setBorderWidth( 1 );
        }
    void draw();

    std::string getText()                           { return text;                  }
    void setText( std::string _text )               { text = _text;                 }

    void setOnClick( fptr_click_event fptr_ )       { onClick = fptr_;              }
    void clickEvent( int x_, int y_, int button, int state );
    void dragEvent ( int x_, int y_ ) {}

protected:
    string text;
    fptr_click_event onClick;
    bool pressed;
};

} // namespace xmx
#endif /* BUTTON_H */
