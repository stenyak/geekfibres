#ifndef TILES_H
#define TILES_H

#include "../tools/portable_sdl.h"
#include "debug.h"
#include "../actors/animacion.h"

#include "math.h"
#include <string>
#include <fstream>
#include <dirent.h>
#include <map>
#include <utility>
/*
NivelDeIntegridad:
BASTANTE_REVENTAO = 0
POCO_REVENTAO = 1
PERFECTO = 2

TipoDeTile:
NUCLEO = 0
REVESTIMIENTO = 1
CUBIERTA = 2
*/
const int VIDA_INICIAL=100;
const int INTEGRIDADES = 3;     //contando con integridad nula, es decir,

   // inexistente --> la integridad se refiere a los !daños del tile.
const int TIPOS = 4;
const int DIMENSION_TILE = 64;  //anchura&altura de los tiles
/*
tipos 0 a 2 -> tiles normales, solidos
tipo 3 -> tiles traspasables
    integridades:
    0 -> inicio de nivel -> tile30.bmp -> 'i'
    1 -> fin de nivel -> tile31.bmp -> 'f'
    2 -> vida -> tile32.bmp -> 'v'
*/

typedef struct
{
    string tileName;
    bool isSolid;               // if a tile is not solid, the player can go through
                                // it. Note: a tile should never be solid and a
                                // powerup at the same time, but it could be not 
                                // solid and not be a powerup (kind of decorative 
                                // tiles)
    string imagesDir;
    string dissolveAnimDir;
    ListaImagenes * tileImages; 

    ListaImagenes * dissolveAnimation;

    float maxBreakableIntegrity;// the maximum tile integrity than can be broken
                                // if the current integrity of the tile is greater
                                // or equal to the maximum integrity, it will be
                                // considered an unbreakable tile.
                                
    float integrityPerLevel;    // each tile can have multiple levels of integrity,
                                // each separated by a certain ammount of integrity
    
    bool isPowerUp;             // indicates if this tile gives any kind of powerup
                                // to the user. Equals to 
                                //      doesIncreaseHealth ||doesIncreaseAmmo || 
                                //      doesIncreaseFirepower || 
                                //      doesIncreasePlayerSpeed et cetera
                                //
    // the ammount these variables will increase the corresponding player atribute
    // will be equal to the tile integrity (the high resolution one).
    bool doesIncreaseHealth;
    bool doesIncreaseAmmo;
    bool doesIncreaseFirepower;
    bool doesIncreasePlayerSpeed;

    bool isEndLevelPoint;
    bool isStartLevelPoint;
}TileData;

class TileBuilder
{
    private:
        static TileData * LoadTileData(string filename);
    public:
        static void LoadTileTypes();
        static TileData * GetTileDataByName(string tileName);
        static map<string,TileData *> tileTypes;
};


class Tile
{
  private:
    string tileType;
    float integrity; // can go from 0 to 3.
    
  public:
       
    Tile (string tileType, int integrityLevel); // 0 is the weakest, 9 the strongest
                                                // although the stronger levels 
                                                // might not exist
    Tile (string tileType, float integrity);

    bool disminuirNivelDeIntegridad (void); //devuelve true si está
                                            // completamente reventao

    bool disminuirNivelDeIntegridad (float damage);//quita al tile la vida indicada
    SDL_Surface *GetImagen (void); 
    
    
    float GetIntegrity();
    void SetIntegrity(float integrity);
    int GetIntegrityLevel();
    string GetType();
    void SetType(string type);
    bool esTraspasable();
    void CrearExplosiones(int x, int y); //TODO remove those X Y params, as they aren't necessary 
};

#endif
