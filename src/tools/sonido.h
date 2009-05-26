#ifndef SONIDO_H
#define SONIDO_H
#include "portable_sdl_mixer.h"
class Sonido
{
	private:
		Mix_Chunk * sonido;
		int ultimoCanal; //�ltimo canal en el que se puso �ste sonido
		
	public:
		Sonido(const char * nombreFichero);
		~Sonido();

		void PlaySound(int volumen=-1);
		

};
#endif
