#include "tools/portable_sdl.h"
#include "tools/portable_sdl_mixer.h"
#include "tools/portable_sdl_ttf.h"
#include "tools/portable_sdl_image.h"
#include "tools/leveldata.h"
#include "juego.h"
#ifndef WIN32
#include <stdio.h>
#endif
#include "editor.h"
#include "stdio.h"
#include "tools/config.h"
#include <math.h>
#include <string>
bool InicializarSDL ();
int Menu ();
void mostrarMenu (const char* imagenFondo );
