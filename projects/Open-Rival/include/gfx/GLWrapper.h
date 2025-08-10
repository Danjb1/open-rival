// On Windows, we have to include the Windows header BEFORE OpenGL
#include "platform/WindowsWrapper.h"

// This must be included before anything else OpenGL-related
#include <GL/glew.h>

// Needed for gluErrorString (and apparently not much else)
#include <GL/glu.h>
