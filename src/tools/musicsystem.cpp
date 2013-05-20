#include "musicsystem.h"
#include <stdlib.h>

Mix_Music * MusicSystem::music; 
int MusicSystem::channel;

void MusicSystem::PlayMusic()
{
#ifndef WIN32
	struct dirent *ep=NULL;
	DIR * dir = opendir((ConfigManager::gamedataPath + string("music")).c_str());

	vector <string> fileNames;

	while((ep=readdir(dir))!=NULL)
	{
		if(ep->d_name[0]!='.') // nos libramos de los archivos ocultos de linux, ./, ../, .svn, etc    
		{
			string fileString = ConfigManager::gamedataPath + string("music/") +string(ep->d_name);                                                                                                              fileNames.push_back(fileString);
		}       
	}       

	int theOne = rand()%fileNames.size();

	music = Mix_LoadMUS (fileNames[theOne].c_str());
	if(music)
		printf("Loaded song:%s\n",fileNames[theOne].c_str());

	Mix_PlayMusic(music, 1);
	
	closedir(dir);
#else           
	// Aqui faltaria implementar el random correspondiente para windows
	music = Mix_LoadMUS ((ConfigManager::gamedataPath + string("music/LIVIN'_INSANITY.MOD")).c_str());
	Mix_PlayMusic ( music, -1 );
	//Mix_RegisterEffect(channel, MusicSystem::FooEffect, MusicSystem::MusicStoppedCallback);                                                                            
#endif

}
void MusicSystem::StopMusic()
{
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}
