#include "tools/portable_sdl.h"
#include "tools/portable_sdl_mixer.h"
#include "tools/portable_sdl_image.h"
#include "tools/leveldata.h"
#include "actors/protagonista.h"
#include "world/mundo.h"
#include "physics/motorfisico.h"
#include "actors/disparo.h"
#include "actors/animacion.h"
#include "tools/musicsystem.h"
#ifndef WIN32
#include <dirent.h>
#include <unistd.h>
#endif
#include "string.h"
#include <string>
#include "stdio.h"
#define MUSICA_POR_DEFECTO true

int Juego ( SDL_Surface * screen, LevelData * level );
void GestionarEventosDeEntrada ( Protagonista * prota, int &finPartida, SDL_Surface * screen, bool * mustPlayMusic );
void aplicarFisicosAlProta ( Protagonista * prota, Mundo & mundo );
void DrawBackground(int poscamaraX, int posCamaray, SDL_Surface * imagenFondo, SDL_Surface * screen);
