#include "protagonista.h"
/***************************************************************
 *                 Constructor por defecto                       
 *  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
 *								
 *  Carga todas las animaciones del prota en memoria, e 		
 *  inicializa todas las variables necesarias para tener al 	
 *  personaje preparado para jugar				
 */							

Protagonista::Protagonista (int * finPartida,CharacterData * charData)
{
	posAnim = 0;
	velX = velY = 0;
	accX = accY = 0;
	vida = 100;
	finDeNivel = finPartida;
	deboMovermeDerecha = deboMovermeIzquierda = deboSaltar = false;
	this->charData = charData;
	
	animActual="animCorrer";

	doh1=new Sonido( (ConfigManager::gamedataPath + string("doh.wav")).c_str());
	doh2=new Sonido( (ConfigManager::gamedataPath + string("doh2.wav")).c_str());
	ladoAndar=1; //empezamos mirando a la derecha
}

/*****************************************************************
 *                        Actualizate                            
 *  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
 *                                                               
 * Actualiza la posiciÛn del personaje seg˙n los valores         
 * actuales de velocidad, y seg˙n si colisiona con alguno de los 
 * tiles cercanos a Èl                                           
 */ 

void Protagonista::Actualizate ()
{
	posAnim++;
	if (int (charData->actions[animActual]->numberOfFrames) <= posAnim)
	{
		posAnim = 0;
	}

	if((!deboMovermeIzquierda && !deboMovermeDerecha) || (deboMovermeIzquierda && deboMovermeDerecha))
	{	
		if((animActual=="animCorrer")||(animActual=="animQuieto"))	
		{
			animActual="animQuieto";
			ladoAndar=1;
		}	
		else
		{
			animActual="animQuietoRev";
			ladoAndar=0;
		}
	}
	else 
	{
		if (deboMovermeIzquierda)
		{
			animActual="animCorrerRev";
			accX -= charData->actions[animActual]->accelX;
			ladoAndar=0;
		}

		if (deboMovermeDerecha)
		{
			animActual="animCorrer";
			accX += charData->actions[animActual]->accelX;
			ladoAndar=1;
		}
	}

	if (deboSaltar && puedoSaltar)
	{
		if(puedoSaltar==2) //hack para el doble salto
			accY =charData->actions[animActual]->jumpForce;
		else
			accY =charData->actions[animActual]->jumpForce*2/3;
		
		deboSaltar = false;
		puedoSaltar--; //hack para el doble salto
	}
}

/***************************************************************\
 *                           Pintate				
 *  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
 *                                                               
 * Renderiza en pantalla al personaje seg˙n la animaciÛn que se	
 * deba usar, y seg˙n la posiciÛn de la c·mara y la posiciÛn del
 * protagonista
 *                                                               
 */
void Protagonista::Pintate (int posCamaraX, int posCamaraY, SDL_Surface * screen)
{
	CharacterAction * action = charData->actions[animActual];
	SDL_Rect * rectanguloTemporal;
	ListaImagenes * anim = action->animationImages;
	rectanguloTemporal = new SDL_Rect;
	rectanguloTemporal->h = anim->GetImagen(posAnim)->h;
	rectanguloTemporal->w = anim->GetImagen(posAnim)->w;
	rectanguloTemporal->x = posX - (posCamaraX - ConfigManager::H_RES/2);
	rectanguloTemporal->y = (posCamaraY + ConfigManager::V_RES/2) - (posY + rectanguloTemporal->h);

	rectanguloTemporal->x += this->getXOffset();
	rectanguloTemporal->y += this->getYOffset();

	SDL_BlitSurface (charData->actions[animActual]->animationImages->GetImagen (posAnim), NULL, screen, rectanguloTemporal);
	this->PintaHud(screen);
	delete rectanguloTemporal;
}

void Protagonista::hackDesplazate (int desplazamientoX, int desplazamientoY)
{
	posX += desplazamientoX;
	posY += desplazamientoY;
}


void Protagonista::disminuirVida(int danyos)
{   
	if ( (vida -= danyos) < 0 ) *finDeNivel = 1;
}

void Protagonista::disminuirVidaTemporizado(int danyos)
{
	disminuirVida(rand()%50==1?danyos:0);
}

void Protagonista::disminuirVida()
{   
	if ( (vida -= 1) < 0 ) *finDeNivel = 1;
}

void Protagonista::aumentarVida()
{   
	if ( (vida += 50) > 100 ) vida = 100;
}

int Protagonista::getVida()
{
	return vida;
}

void Protagonista::setVida(int nuevaVida)
{
	vida = nuevaVida;
	if (vida <= 0) *finDeNivel = 1;
}

void Protagonista::setFinDeNivel()
{
	*finDeNivel = 2;
}


//FIXME WHAT THE HELL IS THIS DOING HERE????????????
void Protagonista::PintaHud(SDL_Surface * screen)
{
    static TTF_Font *fuenteHud=TTF_OpenFont((ConfigManager::gamedataPath + string ("FreeSerifBold.ttf")).c_str(),40);
    SDL_Color colorLetras;
    colorLetras.r=0;
    colorLetras.g=255;
    colorLetras.b=0;
    colorLetras.unused=0;
    char strVida[4];
    snprintf(strVida,4, "%d",vida);
    SDL_Surface * surfaceVida=TTF_RenderText_Solid/*Blended*/(fuenteHud,strVida,colorLetras);
	SDL_Rect * rectVida=new SDL_Rect;    
    
	rectVida->x=ConfigManager::H_RES/20;
	rectVida->y=int(9*ConfigManager::V_RES/10.0);
	rectVida->w=surfaceVida->w;
	rectVida->h=surfaceVida->h;
    
	SDL_BlitSurface(surfaceVida, NULL,screen, rectVida);
    SDL_FreeSurface(surfaceVida);
	delete rectVida;
	
}

void Protagonista::PlayDoh()
{
	static Uint32 lastTime=0; //tiempo desde el ˙ltimo d'oh (para no resultar cansino)
	Uint32 now=SDL_GetTicks();

	if(now-lastTime>250)
	{
		rand()%2?doh1->PlaySound():doh2->PlaySound();
		lastTime=now;
	}	
}

void Protagonista::PleaseGoRight()
{
	deboMovermeDerecha = true;
	ladoAndar=1;
}

void Protagonista::PleaseGoLeft()
{
	deboMovermeIzquierda = true;
	ladoAndar=0;
}

int Protagonista::getBBWidth()
{
	return charData->sizex;
}

int Protagonista::getBBHeight()
{
	return charData->sizey;
}

int Protagonista::getXOffset()
{
	return (charData->actions[animActual]->bboxXOffset + charData->commonXOffset);
}
int Protagonista::getYOffset()
{
	return (charData->actions[animActual]->bboxYOffset + charData->commonYOffset);
}
