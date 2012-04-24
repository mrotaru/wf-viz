#ifndef GUIUTILS_H
#define GUIUTILS_H

#include "utils.h"
using namespace xmx;

extern Color control_bg;
extern Color control_text;
extern Color control_border;

extern const Color WHITE;
extern const Color BLACK;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color GHOST_WHITE;
extern const Color DARK_GREY;

struct BorderInfo
{
    int size[4];
    Color colors[4];
};

#endif /* GUIUTILS_H */
