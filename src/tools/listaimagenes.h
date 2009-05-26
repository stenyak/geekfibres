#ifndef LISTA_IMAGENES_H
#   define LISTA_IMAGENES_H

#   include "portable_sdl.h"
#   include "portable_sdl_image.h"
#   include "config.h"
#   include <vector>
using namespace std;

/***************************************************************\
*                      ListaImagenes				*
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*								*
* Clase utilizada para que cada clase que se vaya a renderizar  *
* en pantalla, tenga sus im�genes bien empaquetadas en un s�lo  *
* objeto.							*
*  								*
\***************************************************************/

class ListaImagenes
{
  public:
    vector < SDL_Surface * >lista;  // vector de la STL que contiene punteros a imagenes (NOT TESTED)

    ListaImagenes (  );         // crea una lista vac�a
    ~ListaImagenes (  );        // el destructor borra autom�ticamente todas las im�genes cargadas

    SDL_Surface * CargarArchivoSVG(char * nombreDir,int ancho, int alto);
    void CargarDirectorio(const char * nombreDir);
    SDL_Surface *CargarArchivoImagen (const char *nombreFichero );   // carga una imagen del disco, y la a�ade por detr�s a "lista"
    SDL_Surface *GetImagen ( int numero )   // devuelve el puntero a la imagen
    {
        numero %= lista.size (  );
        return lista.at ( numero );
    }
    int GetSize (  )
    {
        return lista.size (  );
    }
    int AltoImagenes();
    int AnchoImagenes();
};

#endif
