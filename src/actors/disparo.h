#ifndef DISPARO_H
#    define DISPARO_H

#    include "debug.h"
#    include <vector>
#    include <string>
#    include <math.h>
#    include <algorithm>
#    include <stdio.h>
#    include "../tools/portable_sdl.h"
#    include "../tools/portable_sdl_mixer.h"
#    include "../tools/config.h"
#    include "../tools/listaimagenes.h"
#    include "../actors/animacion.h"
#    include "../tools/normalrand.h"
using namespace std;


class Disparo
{
  private:
    int   posX,
          posY;
    float fposX,
	  fposY;
    
    float velX,
          velY;
    float accY;
    int tiempoVolando;
    bool deboMorir;             //indica si en la siguiente actualización la bala será destruida

    bool origenProta;   //true -> disparado por prota; false -> disparao por malo

    SDL_Rect *rectDisparo;
    static SDL_Surface *imagenDisparo;
    static vector < Disparo * >vectorDisparos;
    static ListaImagenes imagenesExplosion;
  public:
    //constructores
    Disparo ( int posX, int posY, float velX, float velY, float accY, bool disparadoPorProta);

    static void DisparoMultiple(int posX, int posY, float velX, float accY, bool disparadoPorProta, int numDisparos, float dispersionX, float dispersionY);
		    
    //consultoras

    int GetPosX (  );
    int GetPosY (  );
    int GetVelX (  );           //FIXME deprecated desde nacimiento :P
    bool EstasVivo (  );

    //actualizadoras / renderizadoras

    static void ActualizaTodos (  );    //función estática a la clase, que actualiza todos los disparos del prota
    static void MataTodos (  );
    static void PintaTodos ( int posCamaraX, int posCamaraY, SDL_Surface * screen );    //función estática a la clase, que renderiza todos los disparos del prota
    static Disparo *GetDisparo ( int numeroDeDisparo );    //devuelve el disparo pedido
    static int NumeroDisparos ( void ); //devuelve el numero de disparos existentes

    void Actualizate (  );
    void Destruyete (  );
    void Pintate ( int posCamaraX, int posCamaraY, SDL_Surface * screen );
    bool DisparadoPorProta (void);    //true si lo ha disparao el prota
};

#endif
