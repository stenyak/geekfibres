#ifndef MUNDO_H
#define MUNDO_H

#include "stdio.h"
#include "../world/tile.h"
#include "../tools/portable_sdl.h"
#include "../tools/config.h"
#include "../tools/leveldata.h"
#include "../actors/protagonista.h"
#include "../actors/malo.h"

const int ANCHO_MUNDO = 300;
const int ALTO_MUNDO = 40;

class Mundo
{
  private:
    char SearchStringInStoredCharactersTileMap(string tileName);
                
  public:
    Tile * tilesMundo[ANCHO_MUNDO][ALTO_MUNDO];
        // tilesMundo[0][0] corresponde a la esquina inferior izquierda del mundo.
        // tilesMundo[20][20] es algun punto en medio del mundo
        // tilesMundo[ANCHO_MUNDO-1][ALTO_MUNDO-1]es la esquina superior derecha.

    LevelData * levelData;
    // funcion que carga el mundo de un archivo que se pasa como parámetro
    void CargarMundo (LevelData * level);
    void CargarMundoProta (LevelData * level, Protagonista * prota);
    ~Mundo();

    // renderiza todos los tiles del mundo (y nada más que eso).
    void PintaMundo (int posCamaraX, int posCamaraY, SDL_Surface * screen);

    // función que guarda el mundo actual en un archivo que pasado como parámetro
    void GuardarMundo(const char *nombreArchivoMundo);

};
// el archivo debe tener como mucho ANCHO_MUNDO columnas (en cualquier linea)
// y exactamente ALTO_MUNDO filas (ni mas ni menos, o si no $DEITY bajará de los
// cielos en paracaidas en nombre de SDL).

#endif
