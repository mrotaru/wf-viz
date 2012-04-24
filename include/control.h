#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <string>
#include <memory>

#include "gui_surface.h"
#include "window.h"
#include "gui_utils.h"

namespace xmx
{

class Window;

class Control: public GUI_surface
{
public:
    Control():  GUI_surface( 10, 10 ), name( "control" ), draw_borders( true ), border_width(1),
                background_color(0), text_color(0), border_color(0), hovered(false)
                {}
    Control( int x_, int y_, int width_, int height_, std::string name_ = "control", bool draw_borders_ = true )
             :  GUI_surface( x_, y_, width_, height_ ), name( name_ ), draw_borders( draw_borders_ ), border_width(1),
                background_color(0), text_color(0), border_color(0), hovered(false)
                {}
    virtual ~Control() {};

    virtual void draw() = 0;
    virtual void clickEvent( int, int ) = 0;
    void hoverEnterEvent( int, int ) { hovered = true; };
    void hoverLeaveEvent( int, int ) { hovered = false; };
    int getID() { return ID; }

    // setters/getters
    std::string getName()                         { return name; }
    void setName( std::string _name )             { name = _name; }

    Window* getParent()                           { return parent; }
    void setParent(  Window* parent_ )            { parent = parent_; }

    Color* getBackgroundColor()                   { return background_color;   }
    void setBackgroundColor( const Color* color_ ){ background_color = const_cast< Color* >( color_ ); }
    void useBackgroundColor()                     { background_color ?    setColor( background_color ): setColor(control_bg ); }

    Color* getTextColor()                         { return text_color;   }
    void setTextColor( const Color* color_ )      { text_color = const_cast< Color* >( color_ ); }
    void useTextColor()                           { text_color ?          setColor( text_color ): setColor( control_text ); }; 

    Color* getBorderColor()                       { return border_color;   }
    void setBorderColor( const Color* color_ )    { border_color = const_cast< Color* >( color_); }
    void useBorderColor()                         { border_color ?        setColor( border_color ): setColor( control_border ); }

    int getBorderWidth()                          { return border_width;   }
    void setBorderWidth( int width_ )             { border_width = width_; }

    bool isPointInside( int, int );

protected:
    Window* parent;
    std::string name;
    bool draw_borders;
    int border_width;
    Color* background_color;
    Color* text_color;
    Color* border_color;
    bool hovered;
};

} // namespace xmx
#endif /* CONTROL_H */
