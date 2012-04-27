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
        Control( x_, y_, width_, height_ , title_ ), focus( false ), hovered_control( shared_ptr< Control >() ) { 
            titleBar = shared_ptr< Label >( new Label( width_ - 3, 18, title_ ) ); 
            titleBar->setTextColor( &GHOST_WHITE );
            titleBar->setBackgroundColor( &DARK_GREY );
            addControl( titleBar, 1, 1 );   
        }

    virtual ~Window() {};
    void draw();

    void dragEvent ( int, int );
    void clickEvent( int, int, int, int );
    void hoverEnterEvent( int, int );
    void hoverLeaveEvent( int, int );
    void keyPressed( unsigned char, int, int );

    bool hasFocus()     { return focus; }
    void losfFocus()    { titleBar->setBackgroundColor( &DARK_GREY ); focus = false; }
    void giveFocus()    { titleBar->setBackgroundColor( &dcol_EPoints ); focus = true; }

    void addControl( shared_ptr< Control >, int, int );
    void addAutoSizedControl( shared_ptr< Control >, int, int, int, int );
    bool isPointInside( int x_, int y_ ) { return(  x_ >= x && x_ <= x + width && y_ >= y && y_ <= y + height); }
    bool isPointInsideAnyControl( int, int );

protected:
    bool focus;
    static int count;
    int ID;
    shared_ptr< Control > hovered_control;
    shared_ptr< Label > titleBar;
    vector< shared_ptr<Control> > controls;
};

} // namespace xmx
#endif /* WINDOW_H */
