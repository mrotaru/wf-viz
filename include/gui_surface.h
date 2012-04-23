#ifndef GUI_SURFACE_H
#define GUI_SURFACE_H

#include "globals.h"

namespace xmx {

class GUI_surface
{
public:
    // c-tors/d-tors
    //-------------------------------------------------------------------------
    GUI_surface(): 
        x(10), y(10), width(100), height(50) {}
    GUI_surface( int x_, int y_ ):
        x( x_ ), y( y_ ), width(100), height(50) {}
    GUI_surface( int x_, int y_, int width_, int height_ ):
        x( x_ ), y( y_ ), width( width_ ), height( height_ ) {}
    virtual ~GUI_surface() {};

    // methods
    //-------------------------------------------------------------------------
    void draw();
    virtual int getID() { return ID; }

    // In OpenGL, (0,0) is in the bottom left corner; but the GUI is laid out using
    // the top left as (0,0). This function converts the y coordinate as used by
    // the GUI - with 0,0 in the top-left, to OpenGL coordinates - with 0,0 in the
    // bottom-left corner.
    //------------------------------------------------------------------------------
    int toGl( int gui_y )
    {
        return( window_height - ( height + gui_y ) + height );
    }

    // setters/getters
    //-------------------------------------------------------------------------

    // x/y
    int getX()          { return x; }
    void setX( int _x ) { x = _x; }
    int getY()          { return y; }
    void setY( int _y ) { y = _y; }

    // width
    int getWidth()                      { return width; }
    void setWidth( int _width )         { width = _width; }

    // height
    int getHeight()                     { return height; }
    void setHeight( int _height )       { height = _height; }
    
protected:
    int ID;
    int x,y;
    int width, height;
};

} // namespace xmx
#endif /* GUI_SURFACE_H */
