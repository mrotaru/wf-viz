#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "control.h"
#include "GUI_surface.h"

namespace xmx
{

class Window: public Control
{
public:
    Window() {};
    Window( int x_, int y_, int width_, int height_, std::string title_ ):
        Control( x_, y_, width_, height_ , title_ ) {}
    virtual ~Window() {};
    void draw();
    void addControl( boost::shared_ptr<Control>, int, int );

private:
    static int count;
    int ID;
    vector< boost::shared_ptr<Control> > controls;
};

} // namespace xmx
#endif /* WINDOW_H */
