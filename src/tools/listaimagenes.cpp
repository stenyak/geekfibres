#include "listaimagenes.h"
#include "stdio.h"
#include "dirent.h"
#include <algorithm>
#include <string>
//#include "SDL_svg.h"

ListaImagenes::ListaImagenes (  )
{
    //hay algo que hacer??
}

/***************************************************************\
*                    Destructor por defecto			*
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*								*
*  Borra uno a uno todos los elementos del vector, borrando los *
*  SDL_Surface a los que apuntan.				*
* 								*
\***************************************************************/

ListaImagenes::~ListaImagenes (  )
{
    // me da la impresion que el propio tipo vector<> deberia tener sus
    // propias funcinoes de eliminacion y etc... este metodo me parece
    // un poco warro (no offense intended). De toas formas no tengo
    // ganas de investigar ahora... tan solo estoy ech·ndole un vistazo
    // a la estructura del codigo en general (bySTK).
    while ( lista.size (  ) )   //UNTESTED!!!!!!!!!!!!!!!-<--<--<--<--<--<--<--<--<--<
    {
        delete lista[0];
        lista.erase ( lista.begin (  ) );
    }

}

void ListaImagenes::CargarDirectorio(const char * nombreDir)
{

	struct dirent *ep=NULL;
	DIR * dir;
	// there might be a buffer overflow here....
	string finalDir = ConfigManager::gamedataPath + string(nombreDir);
	dir = opendir(finalDir.c_str());
	
	vector <string> fileNames;
	printf("\t\t\t\t- Loading images list: %s\n",finalDir.c_str());
		
	
	while((ep=readdir(dir))!=NULL)
	{
		if(ep->d_name[0]!='.') // nos libramos de los archivos de svn.... 0:-)
		{
			string fileString = finalDir + string("/") +string(ep->d_name);
			fileNames.push_back(fileString);
		}
	}
	
	sort(fileNames.begin(),fileNames.end());

    vector<string>::iterator it;
    for(it=fileNames.begin();it!=fileNames.end();it++)
		this->CargarArchivoImagen((*it).c_str());
	
	// TODO AÒadir comprobacion por si no existe el directorio que buscamos, no hay archivos, o los archivos que hay no son de imagenes
	
}

/***************************************************************\
*                      CargarArchivoImagen			*
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
* 								*
* Carga una imagen que se le pasa como par·metro desde el disco, y
* la aÒade a la lista por detr·s.				*
* Devuelve un puntero a la imagen, o NULL si no ha podido       *
* cargarla                                                      *
* 								*
\***************************************************************/

// UNTESTED!!!!!!!!!!!!!
SDL_Surface *ListaImagenes::CargarArchivoImagen ( const char *nombreFichero )
{
	SDL_Surface *imagen;
    SDL_Surface *imagen1;

    
    if ( !( imagen = IMG_Load ( nombreFichero ) ) )
    {
	printf("No se ha podido cargar la imagen:%s\n",nombreFichero);
        return NULL;            // si es NULL, es que no ha podido cargar la imagen
    }

    static int colorKey = SDL_MapRGB ( imagen->format, 255, 0, 255 );

    SDL_SetColorKey ( imagen, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorKey );
 //   printf("Image size: %ix%i\n",imagen->w,imagen->h);
    if ( !( imagen1 = SDL_DisplayFormat ( imagen ) ) )
    {
        lista.push_back( imagen );
        return imagen;
    }
    else
    {
        lista.push_back( imagen1 );
        delete imagen;
        return imagen1;
    }

}

int ListaImagenes::AltoImagenes()
{
	return lista[0]->h;
}

int ListaImagenes::AnchoImagenes()
{
	return lista[0]->w;
}
