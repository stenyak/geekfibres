#ifndef SONIDO_H
#define SONIDO_H
#include "portable_sdl_mixer.h"
class Sonido
{
	private:
		Mix_Chunk * sonido;
		int ultimoCanal; //último canal en el que se puso éste sonido
		
	public:
		Sonido(const char * nombreFichero);
		~Sonido();

		void PlaySound(int volumen=-1);
		

};
#endif
