#include "personaje.h"

void Personaje::PleaseGoRight ()
{
    deboMovermeDerecha = true;
    ladoAndar=1;
}

void Personaje::PleaseStopRight ()
{
    deboMovermeDerecha = false;
}

void Personaje::PleaseGoLeft ()
{
    deboMovermeIzquierda = true;
    ladoAndar=0;
}

bool Personaje::DoesUserWantToMove()
{
	return (deboMovermeDerecha || deboMovermeIzquierda) && !(deboMovermeDerecha &&deboMovermeIzquierda);
}

void Personaje::PleaseStopLeft ()
{
    deboMovermeIzquierda = false;
}


void Personaje::SetPosVelX (int posicionX, int velocidadX)   //actualiza posX,velX y borra accX (=0)
{
    posX = posicionX;
    velX = velocidadX;
    accX = 0;
}
void Personaje::SetPosVelY (int posicionY, int velocidadY)   //actualiza posY,velY y borra accY (=0)
{
    posY = posicionY;
    velY = velocidadY;
    accY = 0;
}


int Personaje::GetPosX ()
{
    return posX;                //relativo al margen izquierdo
}

int Personaje::GetPosY ()
{
    return posY;                //relativo al suelo
}

int Personaje::GetVelX ()
{
    return velX;
}

int Personaje::GetVelY ()
{
    return velY;                //positivo=sube ; negativo=baja
}

int Personaje::GetAccX ()
{
    return accX;
}

int Personaje::GetAccY ()
{
    return accY;
}
bool Personaje::GetLadoAndar()
{
	return (ladoAndar);
}

bool Personaje::IsTouchingGround()
{
		return isTouchingGround;
}

void Personaje::SetTouchingGround(bool touch)
{
		isTouchingGround = touch;
}
	
Personaje::~Personaje()
{;}

void Personaje::PuedesSaltar ()
{
    puedoSaltar = 2;//true;         //"estoy en el suelo" ->indicado por el motor fisico
}
    
void Personaje::NoPuedesSaltar ()
{
    puedoSaltar = 0; //false
}

void Personaje::PleaseJump ()
{
    if(puedoSaltar)
    deboSaltar = true;
}
                                                                                                                                                                       
                                                                                                                                                                       
