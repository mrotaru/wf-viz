#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <string>
#include <memory>

#include <boost/shared_ptr.hpp>

#include "gui_surface.h"
#include "gui_utils.h"

namespace xmx
{

class Window;

class Control: public GUI_surface
{
public:
    Control(): GUI_surface( 10, 10 ), name( "control" ) {};
    Control( int x_, int y_ , string name_ ): GUI_surface( 10, 10 ), name( name_ ) {}
    virtual ~Control() {};

    virtual void draw() = 0;
    int getID() { return ID; }

    // setters/getters
    std::string getText()               { return text; }
    void setText( std::string _text )   { text = _text; }

protected:
    std::string name;
    std::string text;
    Window* parent;
};

} // namespace xmx
#endif /* CONTROL_H */
