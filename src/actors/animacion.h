#ifndef ANIMACION_H
#define ANIMACION_H

#    include "debug.h"
#    include <vector>
#    include "../tools/portable_sdl.h"
#    include "stdio.h"
#    include "../tools/config.h"
#    include <math.h>
#    include "../tools/listaimagenes.h"
using namespace std;

class Animacion
{
  private:
    int posX,
        posY;
    static vector < Animacion * >vectorAnimaciones;
    ListaImagenes *lista;
    int posicionLista;
    int tamanyoLista;
    bool muerto;
  public:
    //constructores
         Animacion ( int posX, int posY, ListaImagenes * lista );

    //actualizadoras / renderizadoras

    static void MataTodos (  );
    static void PintaTodos ( int posCamaraX, int posCamaraY,
                             SDL_Surface * screen );

    bool EstasMuerto (  );
    void Destruyete (  );
    void Pintate ( int posCamaraX, int posCamaraY, SDL_Surface * screen );

};

#endif
