#ifndef PORTABLE_SDL_TTF_H
#define PORTABLE_SDL_TTF_H

#ifdef WIN32
#include "SDL_ttf.h"
#else
#ifdef __APPLE__
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif
#endif
#endif
