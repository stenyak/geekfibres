#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include "personaje.h"
#include "characterbuilder.h"
class Protagonista : public Personaje
{
  private:
    int * finDeNivel;            //indica si has acabao el nivel actual
    
    Sonido * doh1;
    Sonido * doh2;
  public:
    Protagonista (int *, CharacterData * charData);  //constructor por defecto

    void hackDesplazate (int desplazamientoX, int desplazamientoY);

    void Actualizate ();        //actualiza todas las propiedades de posición y
    void Pintate (int posCamaraX, int posCamaraY, SDL_Surface * screen);    //se renderiza a
    void PintaHud(SDL_Surface * screen);
    void setFinDeNivel();
    void aumentarVida();
    void disminuirVida();
    void disminuirVida(int danyos);
    void disminuirVidaTemporizado(int danyos);
    int getVida();
    void setVida(int nuevaVida);
    void PlayDoh();

    int getBBWidth();
    int getBBHeight();
    int getXOffset();
    int getYOffset();
    
	void PleaseGoRight();
	void PleaseGoLeft();
};

#endif
