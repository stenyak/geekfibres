#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "../tools/portable_sdl.h"
#include "../tools/portable_sdl_ttf.h"
#include "../tools/listaimagenes.h"
#include "../tools/config.h"
#include "../tools/sonido.h"
#include "characterbuilder.h"
#include "debug.h"
#include "stdio.h"
#include "math.h"
#include <string>

class Personaje
{
  protected:
    int accX, accY;             //vectores de aceleracion
    int velX, velY;             //vector de velocidad del protagonista 
    int posX, posY;             //posiciÛn actual del protagonista

    
    string animActual;

    CharacterData * charData;
    
	bool deboMovermeDerecha;
    bool deboMovermeIzquierda;
    int posAnim;                //numero de frame actual de la animaciÛn del personaje

    bool ladoAndar;		//lado hacia el que se pretende mover el personaje (0=izda, 1=dcha)(usado en los disparos)

    bool isTouchingGround;
	bool deboSaltar;
    char puedoSaltar;
    int vida;   		//vida actual, de 0 a 100 
    SDL_Rect *rect;             //rectangulo donde se posiciona el personaje

  public:
    virtual ~Personaje();	
    int GetPosX ();
    int GetPosY ();
    int GetVelX ();
    int GetVelY ();
    int GetAccX ();
    int GetAccY ();
    bool GetLadoAndar();       //hacia dÛnde est· mirando el personaje? 0=izda, 1=dcha

    void PleaseGoRight ();      //pide al protagonista que en la siguiente
    void PleaseStopRight ();
    void PleaseGoLeft ();       //pide al protagonista que en la siguiente
    void PleaseStopLeft ();
    void SetPosVelX (int posicionX, int velocidadX);  //actualiza posX,velX y borra accX (=0)
    void SetPosVelY (int posicionY, int velocidadY);  //actualiza posY,velY y borra accY (=0)

	bool DoesUserWantToMove();

	void PuedesSaltar ();       //llamado normalmente desde el motor fisico, permite al bixo saltar (porque ha tocao suelo, por ejemplo)
    void NoPuedesSaltar ();     //deshabilita el salto.
    void PleaseJump ();         //pide al protagonista que en la siguiente actualizaciÛn
            
	bool IsTouchingGround();
	void SetTouchingGround(bool touch);	

    virtual void Pintate (int posCamaraX, int posCamaraY, SDL_Surface * screen)=0;    //se renderiza a
};

#endif
