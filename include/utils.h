#ifndef UTILS_H
#define UTILS_H

#include <list>
#include <string>

#include <GL/glu.h>
using namespace std;

namespace xmx {

void printText( int nX, int nY, string text );
void printBigText( int nX, int nY, string text );
GLfloat dist( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );

} // namespace xmx
#endif /* UTILS_H */
