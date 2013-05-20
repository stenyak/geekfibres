#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include "leveldata.h"

vector <LevelData *> LevelData::loadedLevels;

LevelData::LevelData(string fileName)
{
	std::ifstream configFile (fileName.c_str());
	
	while(configFile.good())
	{
		// ACHTUNG! We limit line width to 120 chars
		// Should be enough for all our conf needs...
		char lineRead [120];
		configFile.getline(lineRead,120);
		string lineString(lineRead);
		
		int eqPos = lineString.find_first_of('=');
		string name = lineString.substr(0,eqPos);	
		string value = lineString.substr(eqPos+1,lineString.size());
		
		if(name == "levelFilename")
		{
			this->levelFilename = value;
		}
		else if (name == "tileType")
		{
            int commaPos = value.find_first_of(',');
    		string tileTypeChar = value.substr(0,commaPos);	
	    	string tileTypeName = value.substr(commaPos+1,value.size());
  
            std::pair<char,string> rel(tileTypeChar.c_str()[0],tileTypeName);
            this->storedTileChars.insert(rel);
			this->levelTileTypes.push_back(tileTypeName);
		}
		else if (name == "characterType")
		{
            int commaPos = value.find_first_of(',');
    		string characterTypeChar = value.substr(0,commaPos);	
	    	string characterTypeName = value.substr(commaPos+1,value.size());
  
            std::pair<char,string> rel(characterTypeChar.c_str()[0],characterTypeName);
            this->storedCharacterChars.insert(rel); 
			levelCharacters.push_back(CharacterBuilder::GetCharacterByName(characterTypeName));
		}
        else if (name == "mainCharacter")
        {
                int commaPos = value.find_first_of(',');
                string characterTypeChar = value.substr(0,commaPos);
                string characterTypeName = value.substr(commaPos+1,value.size());

                std::pair<char,string> rel(characterTypeChar.c_str()[0],characterTypeName);
                this->storedCharacterChars.insert(rel);

                this->mainCharacter = CharacterBuilder::GetCharacterByName(characterTypeName);
        }
        else if (name == "backgroundFilename")
        {
                this->backgroundFilename = value;
        }
        else if (name == "playerShotsDamage")
        {
                this->playerShotsDamage = (float) atof(value.c_str());
        }
		
	}
	configFile.close();
	loadedLevels.push_back(this);
}

string LevelData::GetLevelFilename()
{
		return this->levelFilename;
}

LevelData * LevelData::GetLevelData(int level)
{
		return loadedLevels[level];
}

int LevelData::GetNumberOfLevelsLoaded()
{
		return loadedLevels.size();
}

string LevelData::GetBackgroundFilename()
{
        return this->backgroundFilename; 
}

float LevelData::GetPlayerShotsDamage()
{
        return this->playerShotsDamage;
}
void LevelData::LoadLevels ( )
{

		struct dirent *ep=NULL;
		DIR * dir;
		
		dir = opendir( (ConfigManager::gamedataPath + string ("levels")).c_str());
		
		vector <string> fileNames;

		while((ep=readdir(dir))!=NULL)
		{
            bool isALevel = string(ep->d_name).find("cfg",1) !=string::npos;
				
			if(ep->d_name[0]!='.' && isALevel)
			{
                string fileString = ConfigManager::gamedataPath + string("levels/") + string(ep->d_name);
                fileNames.push_back(fileString);
            }
		}

		sort(fileNames.begin(),fileNames.end());
        
        vector<string>::iterator it;
        for( it=fileNames.begin();it!=fileNames.end();it++)
        {
            printf("Loading a LevelData from file: %s\n", (*it).c_str());
			new LevelData(*it);
		}
}
