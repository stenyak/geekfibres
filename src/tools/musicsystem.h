#include "portable_sdl.h"
#include "portable_sdl_mixer.h"
#include "config.h"
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include <string>
using namespace std;
class MusicSystem
{
	private:
		static Mix_Music * music;
		static int channel;

	public:

		static void PlayMusic();
		static void StopMusic();

};

