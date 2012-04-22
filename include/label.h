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
    Label(): Control( 10, 10, "label" ), text("label") { cout << "Label constructor" << endl; }
    Label( std::string _text ): Control( 10, 10, "label" ), text( _text ) {}
    Label( std::string _text, int _x, int _y ): Control( _x, _y, "label" ), text( _text ) {}
    void draw();

protected:
    string text;
};

} // namespace xmx
#endif /* LABEL_H */
