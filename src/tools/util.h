#ifndef UTIL_H
#define UTIL_H
#include <portable_gl.h>
#include <portable_glu.h>

#include <portable_sdl.h>
#include <portable_sdl_image.h>
#include <math.h>
// Calcula la media de las normales de 2 poligonos que comparten una arista
GLfloat * calcularProductoVectorialMedio(GLfloat * centro, GLfloat * vertCompartido, GLfloat * vertLateral1, GLfloat * vertLateral2);
GLfloat * calcularProductoVectorial(GLfloat * vert1, GLfloat * vert2, GLfloat * vert3);
GLfloat dotProduct(GLfloat * v1, GLfloat * v2);
void normalizarVector(GLfloat * v);
GLuint loadTextureFromFile(const char * filename, bool mipmaps);

void DrawRect(GLuint textureID, SDL_Rect * rect);


#endif