#include "tools/portable_sdl.h"
#include "tools/portable_sdl_mixer.h"
#include "world/mundo.h"
#include "gui/palette.h"
#include "tools/config.h"

void Editor ( SDL_Surface * screen );
void GestionarEventosEditor ( int &posCursorX, int &posCursorY,
                              bool & finPartida, Mundo & mundoActual, int & posCamaraX, int & posCamaray);
