#ifndef GUIUTILS_H
#define GUIUTILS_H

#include "utils.h"
using namespace xmx;

// standard CSS colors
// http://www.w3schools.com/html/html_colornames.asp
//-----------------------------------------------------------------------------
extern const Color WHITE;
extern const Color BLACK;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color GHOST_WHITE;
extern const Color DARK_GREY;
extern const Color GAINSBORO;
extern const Color LIGHT_GREY;
extern const Color DIM_GREY;
extern const Color DIM_GREY;

// other colors
//-----------------------------------------------------------------------------
extern const Color control_bg;
extern const Color control_text;
extern const Color control_border;
extern const Color MEDIUM_GREY;
extern const Color LIGHTER_GREY;

struct BorderInfo
{
    int size[4];
    Color colors[4];
};

#endif /* GUIUTILS_H */
