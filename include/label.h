#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <iostream>
using namespace std;

#include "gui_surface.h"
#include "control.h"

namespace xmx
{

class Label: public Control
{
public:
    Label(): Control( 10, 10, 100, 50, "label", false ), text("label") {}
    Label( std::string text_ ): Control( 10, 10, 100, 50, "label", false ), text( text_ ) {}
    Label( int x_, int y_, std::string text_ ): Control( x_, y_, 100, 50, "label", false ), text( text_ ) {}
    void draw();

protected:
    string text;
};

} // namespace xmx
#endif /* LABEL_H */
