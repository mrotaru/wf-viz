#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include "gui_utils.h"
#include "control.h"

namespace xmx {

typedef struct Point2D      { double x; double y;           } Point2D;
typedef struct LineString2D { vector< Point2D > points;     } LineString2D;
typedef struct BoundingBox  { float maxX, maxY, minX, minY; } BoundingBox;

class MapDisplay: public Control
{
public:
    MapDisplay( int width_, int height_, std::string text_ ):
        Control( 0, 0, width_, height_, "mapdisplay", true ),
        shapefile( "" ), file_loaded( false ), scale(1),
        map_width( 0 ), map_height( 0 ), map_offset_x( 0 ), map_offset_y( 0 ),
        filled_polygons( false )
        {   setTextColor( &BLACK );
            setBackgroundColor( &GAINSBORO );
            setBorderColor( &LIGHT_GREY );
            setBorderWidth( 1 );
        }
    void draw();

    void loadFromShapefile( std::string );

    // events
    //--------------------------------------------------------------------------
    void clickEvent( int x_, int y_, int button, int state ) {}
    void dragEvent ( int, int );
    void keyPressed( unsigned char, int, int );

    // property get/set
    //--------------------------------------------------------------------------
    void setScale( float scale_ )           { scale = scale_; }
    float getScale()                        { return scale;   }

    void setMapOffsetX( int map_offset_x_ ) { map_offset_x = map_offset_x_; }
    float getMapOffsetX()                   { return map_offset_x;          }

    void setMapOffsetY( int map_offset_y_ ) { map_offset_y = map_offset_y_; }
    float getMapOffsetY()                   { return map_offset_y;          }

    void setDisplayFilledPolygons( bool filled_polygons_ ) { filled_polygons = filled_polygons_; }
    bool getDisplayFilledPolygons()                        { return filled_polygons; }

protected:
    std::string shapefile;
    bool file_loaded;
    float scale;
    float map_width;
    float map_height;
    float map_offset_x;
    float map_offset_y;
    vector< Point2D >       points;
    vector< LineString2D >  lines;
    vector< LineString2D >  polygons;
    BoundingBox             map_BB;
    bool filled_polygons;
    map< int, string > index;
};

} // namespace xmx
#endif /* MAPDISPLAY_H */
