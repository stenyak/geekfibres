#ifndef PORTABLE_GL_H
#define PORTABLE_GL_H

#ifdef WIN32
#include "gl.h"
#elif defined LINUX
#include <GL/gl.h>
#else 
#include <OpenGL/gl.h>
#endif


#endif
