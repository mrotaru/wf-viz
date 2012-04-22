#ifndef GUI_SURFACE_H
#define GUI_SURFACE_H

namespace xmx {

class GUI_surface
{
public:
    // c-tros/d-tors
    //-------------------------------------------------------------------------
    GUI_surface(): x(10), y(10) {}
    GUI_surface( int x_, int y_ ): x( x_ ), y( y_ ) {}
    virtual ~GUI_surface() {};

    // methods
    //-------------------------------------------------------------------------
    void draw();
    virtual int getID() { return ID; }

    // setters/getters
    //-------------------------------------------------------------------------

    // x/y
    int getX()          { return x; }
    void setX( int _x ) { x = _x; }
    int getY()          { return y; }
    void setY( int _y ) { y = _y; }

    // widht
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
