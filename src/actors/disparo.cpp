#include "disparo.h"
#include <stdlib.h>
SDL_Surface *Disparo::imagenDisparo = NULL;
ListaImagenes Disparo::imagenesExplosion;

vector < Disparo* >Disparo::vectorDisparos;

void Disparo::DisparoMultiple( int posX, int posY, float velX, float accY, bool disparadoPorProta,int numProyectiles,float dispersionX, float dispersionY )
{
	float newVelX,newVelY;
	float r,phi;
	for(int i=0;i<numProyectiles;i++)
	{
		r=dispersionX*dispersionY/10.0;
		phi = rand()%360000/1000;
		phi/=dispersionY;
		phi-=45;	
		newVelY = sin(phi*3.141592653589*2/360)*normalrand((int)(-r*dispersionY),(int)(r*dispersionY))/5.0;
		newVelX = velX*velX / (cos(phi*3.141592653589*2/360)*velX+normalrand((int)(-r*dispersionX),(int)(r*dispersionX))/50.0);
		new Disparo(posX,posY,newVelX,newVelY,accY,disparadoPorProta);
	}
}
	
Disparo::Disparo( int posX, int posY, float velX, float velY, float accY, bool disparadoPorProta )
{
    if ( imagenDisparo == NULL )
    {
        imagenDisparo = IMG_Load ( (ConfigManager::gamedataPath + string ("balaprota.png")).c_str() );
	imagenesExplosion.CargarDirectorio("explosions/1");

	SDL_SetColorKey ( imagenDisparo, SDL_SRCCOLORKEY|SDL_RLEACCEL,
                          SDL_MapRGB ( imagenDisparo->format, 255, 0, 255 ) );
    }
    this->accY=accY;
    this->fposX = posX;
    this->fposY = posY;
    this->velY = velY;
    this->velX = velX;
    vectorDisparos.push_back( this );
    tiempoVolando = 0;
    origenProta = disparadoPorProta;

    deboMorir=false;
}

int Disparo::GetPosX (  )
{
    return ( posX );
}

int Disparo::GetPosY (  )
{

    return ( posY );
}

int Disparo::GetVelX (  )
{
    return ( (int)velX );
}

bool Disparo::EstasVivo (  )
{
    return ( !deboMorir );
}

void Disparo::Destruyete (  )
{
    new Animacion ( posX, posY, &imagenesExplosion );
	deboMorir = true;
}

void Disparo::Pintate ( int posCamaraX, int posCamaraY,
                             SDL_Surface * screen )
{
    SDL_Rect *rectDisparo = new SDL_Rect;

    rectDisparo->x = posX - ( posCamaraX - ConfigManager::H_RES / 2 ) - imagenDisparo->w / 2;
    rectDisparo->y = ( posCamaraY + ConfigManager::V_RES / 2 ) - posY - imagenDisparo->h / 2;
    rectDisparo->w = imagenDisparo->w;
    rectDisparo->h = imagenDisparo->h;
    SDL_BlitSurface ( imagenDisparo, NULL, screen, rectDisparo );

}

void Disparo::Actualizate (  )
{
    fposX += velX;
    velY+=accY;
    fposY += velY;
    posX=(int)fposX;
    posY=(int)fposY;
    tiempoVolando++;
    if ( tiempoVolando > 100 )
        this->Destruyete (  );
}

//Borra todos los disparos que no estÈn vivos. Luego actualiza las posiciones de todos
//los que estÈn vivos, y llama al motor fÌsico para que compruebe cuales colisionan
void Disparo::ActualizaTodos (  )
{
    // TODO change this for an iterator-based loop
	for(int u=0;u<(int)vectorDisparos.size();u++)
		if(vectorDisparos.at(u)!=NULL&&!vectorDisparos.at(u)->EstasVivo())
		{
			delete vectorDisparos.at(u);
			vectorDisparos.at(u)=NULL;
		}
	unsigned int i=0;
    
	while(i<vectorDisparos.size())
	{
		if(vectorDisparos.at(i)==NULL)
		{	
			vectorDisparos.erase(vectorDisparos.begin()+i);
			i--;
		}
		i++;
	}
    
    vector<Disparo *>::iterator it;
    for(it=vectorDisparos.begin();it!=vectorDisparos.end();it++)
        (*it)->Actualizate (  );  
}

void Disparo::PintaTodos ( int posCamaraX, int posCamaraY,
                                SDL_Surface * screen )
{
    for(vector<Disparo *>::iterator it=vectorDisparos.begin();it!=vectorDisparos.end();it++)
        (*it)->Pintate ( posCamaraX, posCamaraY, screen );
}

void Disparo::MataTodos (  )
{
    for(vector<Disparo *>::iterator it=vectorDisparos.begin();it!=vectorDisparos.end();it++)
        delete (*it);

    vectorDisparos.erase ( vectorDisparos.begin (  ), vectorDisparos.end (  ) );
}

Disparo*Disparo::GetDisparo ( int numeroDeDisparo )
{
    return vectorDisparos.at ( numeroDeDisparo );
}

int Disparo::NumeroDisparos ( void )
{
    return vectorDisparos.size (  );
}

bool Disparo::DisparadoPorProta(void)
{
    return origenProta;
}
