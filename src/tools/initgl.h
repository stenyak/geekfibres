#ifndef INITGL_H
#define INITGL_H
#include <portable_gl.h>
#include <portable_glu.h>
#include <SDL.h>

using namespace std;
int InitGL (SDL_Surface * surface, int windowWidth, int windowHeight, int windowDepth);
void SetPerspectiveMode(int windowWidth, int windowHeight);
void SetOrthoMode(int windowWidth, int windowHeight);
#endif
