#ifndef MALO_H
#define MALO_H

#define ANCHURA_MALO_BB 55
#define ALTURA_MALO_BB 105	//123
#define OFFSET_X_MALO 15
#define OFFSET_Y_MALO 0
#define ACELERACION_SALTO_MALO 120
#define DANYO_BALAS 1
#define ACELERACION_X_MALO 5

#include "../actors/personaje.h"
#include "../actors/protagonista.h"
#include "../world/mundo.h"
#include "../actors/disparo.h"

#include <vector>
#include <math.h>

class Malo : public Personaje
{
  private:
    static vector < Malo *> listaMalos;
    
    bool muerto;
    bool puedoIrIzquierda;
    bool puedoIrDerecha;
    bool deboDisparar;

    // New stuff for action-driven AI
    
    CharacterData * charData;
    string currentAction;
    int currentAnimationFrame;
    
  public:
    Malo (int posX, int posY, CharacterData * charData);  //constructor por defecto

    void Actualizate (int camaraX, int camaraY, Protagonista * prota);   
    void Pintate (int posCamaraX, int posCamaraY, SDL_Surface * screen);    
    bool EstasMuerto();
    void Muerete();
    void Dispara();
    void DisminuyeVida(int danyo);
    
    void InteligenciaArtificial(Protagonista * prota); //inteligencia artificial; llamado desde Actualizate;
    void PuedesIrDerecha();
    void PuedesIrIzquierda();
    void NoPuedesIrDerecha();
    void NoPuedesIrIzquierda();
    
    static int GetNumeroDeMalos(); //devuelve el numero de malos que hay ahora mismo
    static Malo * GetMaloNumero(int numeroMalo);

    static void PintaTodos(int posCamaraX, int posCamaraY, SDL_Surface * screen);
    static void ActualizaTodos(int camaraX, int camaraY, Protagonista * prota);
    static void MataTodos();
};

#endif
