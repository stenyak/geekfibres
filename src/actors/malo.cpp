#include "malo.h"

vector < Malo * >Malo::listaMalos;

/***************************************************************\
*                 Constructor por defecto                       *
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                                               *
*  Carga todas las animaciones del prota en memoria, e          *
*  inicializa todas las variables necesarias para tener al      *
*  personaje preparado para jugar                               *
*                                                               *
\***************************************************************/

Malo::Malo ( int posX, int posY, CharacterData * charData )
{
    posAnim = 0;
    velX = 0;
    velY = 0;
    accX = 0;
    accY = 0;
    this->posX = posX;
    this->posY = posY;
    this->vida=200;
    
    this->charData = charData;
    deboMovermeDerecha = deboMovermeIzquierda = deboSaltar = deboDisparar = true;
	
    listaMalos.insert ( listaMalos.end (  ), this );

    rect = new SDL_Rect;
    rect->w = charData->actions["animPatrullar"]->animationImages->AnchoImagenes();
    rect->h = charData->actions["animPatrullar"]->animationImages->AltoImagenes();
    muerto = false;
    animActual="animPatrullar";
    
    puedoIrIzquierda = true;
    puedoIrDerecha = true;
    puedoSaltar=1;
    this->PleaseGoLeft();
    this->PleaseStopRight();
}

// This will be the core of the new AI system
void SelectNextAction()
{
    // Recollect all the important data about the system state:
    // - Is the player in direct line-of-sight?
    // - Is the player near of far from this character?
    // - Has the character bumped into something?
    //      - Could this character jump over that "something"?
    
    // Now, taking into account the system state, the proper actions are
    // selected among all the available
    
    // Taking the probabilities specified in each action, a random action is
    // selected between the selected ones.
    
}

void Malo::InteligenciaArtificial (Protagonista * prota)
{
    static int distanciaV;
    static int diferenciaV;
    static int distanciaH;
    static int diferenciaH;
    
    diferenciaV = posY - prota->GetPosY();
    if (diferenciaV < 0)
    {
        distanciaV = -diferenciaV;
    }else{
        distanciaV = diferenciaV;
    }
    diferenciaH = posX - prota->GetPosX();
    if (diferenciaH < 0)
    {
        distanciaH = -diferenciaH;
    }else{
        distanciaH = diferenciaH;
    }
    
    if (distanciaV < prota->getBBHeight() /*DIMENSION_TILE*/ && distanciaH < DIMENSION_TILE)
    {
        this->PleaseStopRight();
        this->PleaseStopLeft();
        if (rand()%5!=1)
        {
            this->Dispara();
        }
    }else
    if (distanciaV < (prota->getBBHeight()/DIMENSION_TILE+2)*DIMENSION_TILE )
    {
        // esta mas o menos cerca en el eje Y
        if (diferenciaH > 0)
        {
            // prota a la izquierda de enemigo
            this->PleaseGoLeft();
            this->PleaseStopRight();
            if(puedoIrIzquierda)
            {
                // puede acercase al prota
                if (rand()%100==1)
                {
                    if (rand()%15==1){this->Dispara();}
                }else{
                    if (rand()%200==1){this->PleaseJump();}
                }
            }else{
                // no puede acercarse al prota
                if (puedoSaltar==2)
                {
                    // esta en el suelo
                    this->PleaseJump();
                }else{
                    // esta saltando
                    if (rand()%150==1){this->PleaseJump();}
                    if (rand()%50==1){this->Dispara();}
                }
            }
        }else{
            // prota a la derecha del enemigo
            this->PleaseGoRight();
            this->PleaseStopLeft();
            if(puedoIrDerecha)
            {
                // puede acercase al prota
                if (rand()%100==1)
                {
                    if (rand()%15==1){this->Dispara();}
                }else{
                    if (rand()%200==1){this->PleaseJump();}
                }
            }else{
                // no puede acercase al prota
                if (puedoSaltar==2)
                {
                    // esta en el suelo
                    this->PleaseJump();
                }else{
                    // esta saltando
                    if (rand()%150==1){this->PleaseJump();}
                    if (rand()%50==1){this->Dispara();}
                }
            }
        }
    }else{
        // esta mas o menos lejos en el eje Y, sigue con lo suyo
        if (puedoIrIzquierda && !puedoIrDerecha && puedoSaltar)
        {
            if (rand()%2 && puedoSaltar)
            {
                this->PleaseStopRight();
                this->PleaseGoLeft();
            }else{
                this->PleaseJump();
            }
        }
        if (puedoIrDerecha && !puedoIrIzquierda && puedoSaltar)
        {
            if (rand()%2 && puedoSaltar)
            {
                this->PleaseStopLeft();
                this->PleaseGoRight();
            }else{
                this->PleaseJump();
            }
        }
        if ((!puedoIrIzquierda) && (!puedoIrDerecha) && puedoSaltar)
        {
            this->PleaseJump();
        }
        if ((puedoIrIzquierda) && (puedoIrDerecha))
        {
            if (rand()%25==1 && puedoSaltar){this->PleaseJump();}
            if (rand()%25==1 //&& puedoSaltar==2
            ){this->Dispara();} // FIXME: Por quÈ dispara cuando ni siquiera esta a la altura?
        }
    }
    
}
void Malo::Dispara()
{
    if ((animActual!="animBurp") && (animActual!="animBurpRev"))
    {
        deboDisparar = true;
    }
}

void Malo::Actualizate (int camaraX, int camaraY,Protagonista * prota )
{
    // We only process the enemies more or less inside the screen (in fact, 
    // inside a screen of twice the size of the real one).
    if ((abs(this->GetPosX() - camaraX) > ConfigManager::H_RES) || 
        (abs(this->GetPosY() - camaraY) > ConfigManager::V_RES))
    {
	    return;
    }
    
    this->InteligenciaArtificial(prota);
    
    posAnim++;
    
    CharacterAction * action = charData->actions[animActual];
    
    if (action->doesFire && action->frameToFire == posAnim)
    {
	    Disparo::DisparoMultiple ( GetPosX (  ) + 60,
			    GetPosY (  ) + 90,
			    (action->isFacingRight)?+30 :-30,
			    0/*4*(int)balasUp-4*(int)balasDown*/,
			    false,
			    action->shotBullets,
			    action->dispersionX,
			    action->dispersionY
			    );
    }
    
    if(deboDisparar && !action->doesFire)
    {
        posAnim=0;
        deboDisparar = false;
        if(deboMovermeDerecha)
            animActual="animBurp";
        else
            animActual="animBurpRev";
    }
    
    if (posAnim > action->numberOfFrames)
    {
        posAnim = 0;
		if(action->doesFire) // so that fire actions are not repeated over and over
			animActual="animPatrullar";
    }

    if ( deboMovermeIzquierda && !action->doesFire)
        animActual="animPatrullarRev";

    if ( deboMovermeDerecha && !action->doesFire)
		animActual="animPatrullar";

    accX += action->accelX * (action->isFacingRight?1:-1);

    if (deboSaltar && puedoSaltar)
    {
        if(puedoSaltar==2) //hack para el doble salto
            accY =ACELERACION_SALTO_MALO;//+= ACELERACION_SALTO_PROTA;
        else
            accY =ACELERACION_SALTO_MALO*2/3;
        
        deboSaltar = false;
        puedoSaltar--;//=false; //hack para el doble salto
    }
}

/***************************************************************\
*                           Pintate	                            *
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                                               *
* Renderiza en pantalla al personaje seg˙n la animaciÛn que se	*
* deba usar, y seg˙n la posiciÛn de la c·mara y la posiciÛn del *
* protagonista                                                  *
*                                                               *
\***************************************************************/

void Malo::Pintate ( int posCamaraX, int posCamaraY, SDL_Surface * screen )
{
    ListaImagenes * anim = charData->actions[animActual]->animationImages;
    SDL_Rect * rectanguloTemporal;
    rectanguloTemporal = new SDL_Rect;
    rectanguloTemporal->x = posX - ( posCamaraX - ConfigManager::H_RES / 2 );
    rectanguloTemporal->y = ( posCamaraY + ConfigManager::V_RES / 2 ) - ( posY + rect->h );
    rectanguloTemporal->h = anim->GetImagen(posAnim)->h;
    rectanguloTemporal->w = anim->GetImagen(posAnim)->w;
    SDL_BlitSurface ( anim->GetImagen ( posAnim ), NULL, screen, rectanguloTemporal );
    delete rectanguloTemporal;
}

int Malo::GetNumeroDeMalos (  )
{
    return ( listaMalos.size (  ) );
}

Malo *Malo::GetMaloNumero ( int numeroMalo )
{
    return ( listaMalos[numeroMalo] );
}

void Malo::PintaTodos ( int posCamaraX, int posCamaraY, SDL_Surface * screen )
{
    vector<Malo *>::iterator it;
    for(it=listaMalos.begin();it!=listaMalos.end();it++)
        (*it)->Pintate ( posCamaraX, posCamaraY, screen );
}

void Malo::ActualizaTodos (int camaraX, int camaraY, Protagonista * prota )
{
    // TODO change this for an iterator-based loop
    for ( int c = 0; c < ( int ) listaMalos.size (  ); c++ )
        if ( listaMalos[c]->EstasMuerto (  ) )
        {
            delete listaMalos[c];

            listaMalos.erase ( listaMalos.begin (  ) + c );
            c--;
        }
            
    vector<Malo *>::iterator it;
    for(it=listaMalos.begin();it!=listaMalos.end();it++)
        (*it)->Actualizate (camaraX, camaraY, prota);
    
}

bool Malo::EstasMuerto (  )
{
    return ( muerto );
}

void Malo::Muerete (  )
{
   for(int i=0;i<6;i++)
      	new Animacion(posX+rand()%50,posY+rand()%100, charData->actions["animMorir"]->animationImages);
    
    muerto = true;
}

void Malo::MataTodos (  )
{
    while ( listaMalos.size (  ) > 0 )
    {
        delete listaMalos[0];
        listaMalos.erase ( listaMalos.begin (  ) );
    }
}

void Malo::PuedesIrDerecha()
{
    puedoIrDerecha = true;
}

void Malo::PuedesIrIzquierda()
{
    puedoIrIzquierda = true;
}

void Malo::NoPuedesIrDerecha()
{
    puedoIrDerecha = false;
}

void Malo::NoPuedesIrIzquierda()
{
    puedoIrIzquierda = false;
}

void Malo::DisminuyeVida(int danyo)
{
	vida-=danyo;
	if(vida<=0)
		this->Muerete();
}
