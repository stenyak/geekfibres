#include "sonido.h"
Sonido::Sonido(const char * nombreFichero)
{
	sonido= Mix_LoadWAV(nombreFichero);
	if(!sonido)
	{
		printf("No se pudo abrir el archivo de audio %s\n", nombreFichero);
	}

}

Sonido::~Sonido()
{
	Mix_FreeChunk(sonido);
}


void Sonido::PlaySound(int volumen)
{
	
	ultimoCanal=Mix_PlayChannel(-1,sonido,0);	
	if(volumen!=-1)
		Mix_Volume(ultimoCanal, volumen);

}
		
		


