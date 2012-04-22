#include <boost/shared_ptr.hpp>

#include "window.h"
#include "control.h"

namespace xmx
{

Window::Window()
{
}

Window::~Window()
{
}

void Window::addControl( boost::shared_ptr<Control> ctrl )
{
    controls.push_back( ctrl );
}

} // namespace xmx
