#include "animacion.h"

vector < Animacion * >Animacion::vectorAnimaciones;

Animacion::Animacion ( int posX, int posY, ListaImagenes * lista )
{
    this->posX = posX;
    this->posY = posY;
    this->lista = lista;
    vectorAnimaciones.insert ( vectorAnimaciones.end (  ), this );
    tamanyoLista = lista->GetSize (  );
    muerto = false;
    posicionLista = 0;
}

bool Animacion::EstasMuerto (  )
{
    return ( muerto );
}

void Animacion::Pintate ( int posCamaraX, int posCamaraY, SDL_Surface * screen )
{

    SDL_Surface *imagen = lista->GetImagen ( posicionLista );

    posicionLista++;
    if ( posicionLista >= tamanyoLista )
        muerto = true;
    SDL_Rect *rect = new SDL_Rect;

    rect->x = posX - ( posCamaraX - ConfigManager::H_RES / 2 ) - imagen->w / 2;
    rect->y = ( posCamaraY + ConfigManager::V_RES / 2 ) - posY - imagen->h / 2;
    rect->w = imagen->w;
    rect->h = imagen->h;
    SDL_BlitSurface ( imagen, NULL, screen, rect );

}

void Animacion::PintaTodos ( int posCamaraX, int posCamaraY,
                             SDL_Surface * screen )
{
    //borramos las animaciones que estÈn muertas
    // TODO change this for an iterator-based loop
    for ( int q = 0; q < ( int ) vectorAnimaciones.size (  ); q++ )
        if ( vectorAnimaciones[q]->EstasMuerto (  ) )
        {
            delete vectorAnimaciones[q];

            vectorAnimaciones.erase ( vectorAnimaciones.begin (  ) + q );
            q--;                //Èsto es para evitar que se salte elementos al borrar uno
        }
            
    vector<Animacion *>::iterator it;
    for(it=vectorAnimaciones.begin();it!=vectorAnimaciones.end();it++)
        (*it)->Pintate ( posCamaraX, posCamaraY, screen );

}

void Animacion::MataTodos (  )
{
    vector<Animacion *>::iterator it;
    for(it=vectorAnimaciones.begin();it!=vectorAnimaciones.end();it++)
        delete (*it);

    vectorAnimaciones.clear();
}
