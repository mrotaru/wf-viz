#ifndef WINDOW_H
#define WINDOW_H

#include <boost/shared_ptr.hpp>

#include "control.h"
#include "GUI_surface.h"

namespace xmx
{

class Window: public GUI_surface
{
public:
    Window();
    virtual ~Window();
    void addControl( boost::shared_ptr<Control> );

private:
    static int count;
    int ID;
    vector< boost::shared_ptr<Control> > controls;
};

} // namespace xmx
#endif /* WINDOW_H */
