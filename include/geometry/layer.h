#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <list>
using namespace std;

#include <boost/shared_ptr.hpp>

#include "Shape.h"
#include "Group.h"
#include "Layer.h"

namespace xmx {

class Layer
{
public:
    string name;
    static int count;

    list< boost::shared_ptr<Group    > > groups;
    list< boost::shared_ptr<Shape    > > shapes;
    list< boost::shared_ptr<Primitive> > primitives;

    GLint getNumberOfGroups()       { return groups.size();     }
    GLint getNumberOfShapes()       { return shapes.size();     }
    GLint getNumberOfPrimitives()   { return primitives.size(); }

    void addShape( Shape& );
    void addGroup( Group& );
    void addPrimitive( Primitive& );

    void loadFromFile( string filename );
    void draw();
};

} // namespace xmx
#endif /* LAYER_H */
