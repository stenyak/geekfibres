#ifndef PORTABLE_SDL_MIXER_H
#define PORTABLE_SDL_MIXER_H

#ifdef WIN32
#include "SDL_mixer.h"
#else
#ifdef __APPLE__
#include <SDL_mixer/SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif
#endif
#endif
