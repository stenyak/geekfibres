#ifndef LEVELDATA_H
#define LEVELDATA_H
#include <stdio.h>
#include <map>
#include <vector>
#include <dirent.h>
#include <string>

#include "../actors/characterbuilder.h"

using namespace std;
class LevelData
{
	private:
		static vector <LevelData *> loadedLevels;
		string levelFilename;
        string backgroundFilename;
        vector <string> levelTileTypes; //we have 12 different tiles
        float playerShotsDamage;
	public:
        map <char,string> storedTileChars;
        map <char,string> storedCharacterChars;

        vector <CharacterData *> levelCharacters;
		CharacterData * mainCharacter;
		static void LoadLevels();
		static LevelData * GetLevelData(int level);
		static int GetNumberOfLevelsLoaded();

		LevelData (string fileName);
		LevelData (char * fileName);
        
        string GetBackgroundFilename();
		string GetTileFilename(int tileType);
		string GetLevelFilename();
        float GetPlayerShotsDamage();
};
#endif
