#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

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
        Control( x_, y_, width_, height_ , title_ ) { 
            titleBar = shared_ptr< Label >( new Label( width_ - 3, 18, title_ ) ); 
            titleBar->setBackgroundColor( &dcol_EPoints );
            addControl( titleBar, 1, 1 );   
        }

    virtual ~Window() {};
    void draw();
    void addControl( boost::shared_ptr<Control>, int, int );

private:
    static int count;
    int ID;
    shared_ptr< Label > titleBar;
    vector< boost::shared_ptr<Control> > controls;
};

} // namespace xmx
#endif /* WINDOW_H */
