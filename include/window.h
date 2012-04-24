#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <memory>
using std::shared_ptr;

#include "GUI_surface.h"
#include "control.h"
#include "label.h"

namespace xmx
{

class Window: public Control
{
public:
    Window() {};
    Window( int x_, int y_, int width_, int height_, std::string title_ ):
        Control( x_, y_, width_, height_ , title_ ), focus( false ) { 
            titleBar = shared_ptr< Label >( new Label( width_ - 3, 18, title_ ) ); 
            titleBar->setTextColor( &GHOST_WHITE );
            titleBar->setBackgroundColor( &DARK_GREY );
            addControl( titleBar, 1, 1 );   
        }

    virtual ~Window() {};
    void draw();
    void addControl( shared_ptr<Control>, int, int );
    bool hasFocus()     { return focus; }
    void losfFocus()    { titleBar->setBackgroundColor( &DARK_GREY ); focus = false; }
    void giveFocus()    { titleBar->setBackgroundColor( &dcol_EPoints ); focus = true; }

private:
    bool focus;
    static int count;
    int ID;
    shared_ptr< Label > titleBar;
    vector< shared_ptr<Control> > controls;
};

} // namespace xmx
#endif /* WINDOW_H */
