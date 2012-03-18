#include <string>

#include <GL/glu.h>

#include "globals.h"
#include "utils.h"
using namespace xmx;

// these are set in the main function, when the window is created
int window_width =  0;
int window_height = 0;

// these are read from the VERSION file at compile time
//std::string VERSION    = "?";
//std::string BUILD_ID   = "?";
//std::string BUILD_TIME = "?";

// optput configuration
bool SHOW_BEZIER_POINTS             = false;
bool SHOW_BEZIER_BOUNDING_BOX       = false;
bool SHOW_BEZIER_CONTROL_POINT_LINES= false;
bool SHOW_FRAME_NUMBER              = false;
bool SHOW_FPS                       = true;

// debug flags
bool DEBUG_CTOR_DTOR                = false;
bool DEBUG_BEZIER_BOUNDING_BOX      = false;
bool DEBUG_TRANSF_ROTATE            = false;
bool DEBUG_TRANSF_MOVE              = false;
bool DEBUG_TRANSF_SCALE             = false;
bool DEBUG_INIT_BEZIER_BOUNDING_BOX = false;
bool DEBUG_POV_LOADING              = false;
bool DEBUG_FEEDBACK_TOKENS          = false;
bool DEBUG_SHAPE_BEZIER_DRAW        = false;
bool DEBUG_ANIMATIONS               = false;
bool DEBUG_MORPHING                 = false;

bool STRICT_POV                     = true;

// default colors
Color dcol_CPoints     ( 0.0f, 0.5f, 0.0f );
Color dcol_EPoints     ( 0.0f, 0.0f, 0.5f );
Color dcol_ECLines     ( 0.5f, 0.5f, 0.5f );
Color dcol_BezierBB    ( 0.5f, 0.1f, 0.7f );
Color dcol             ( 1.0f, 0.0f, 0.0f );
Color dcol_Clear       ( 0.0f, 0.0f, 0.03f );
GLfloat d_line_width   = 1.0f;

// frames per second
GLint FPS = 50;
