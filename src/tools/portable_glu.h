#ifndef PORTABLE_GLU_H
#define PORTABLE_GLU_H

#ifdef WIN32
#include "glu.h"
#elif defined LINUX
#include <GL/glu.h>
#else 
#include <OpenGL/glu.h>
#endif


#endif
