#include "../actors/characterbuilder.h"

map <string,CharacterData *> CharacterBuilder::characterTypes;


void CharacterBuilder::LoadCharacterTypes()
{
   			struct dirent *ep=NULL;
			DIR * dir;
			string dirstring= ConfigManager::gamedataPath + string ("characters/");

			dir = opendir(dirstring.c_str());

			while((ep=readdir(dir))!=NULL)
			{
				bool isAConfFile = string(ep->d_name).find("cfg",1) !=string::npos;

				if(ep->d_name[0]!='.' && isAConfFile)
				{
					string fileString = dirstring+string(ep->d_name);
					CharacterBuilder::LoadCharacter(fileString);
		                }
			}
}

CharacterData * CharacterBuilder::GetCharacterByName(string name)
{
        return characterTypes[name];
}

CharacterData * CharacterBuilder::LoadCharacter(string fileName)
{
	std::ifstream configFile (fileName.c_str());

	CharacterData * charData = new CharacterData;
	printf("\t- New character from: %s\n",fileName.c_str());
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

		// TODO Add checks for file existence and file correctness everywhere
		if (name == "actionDir"){
			struct dirent *ep=NULL;
			DIR * dir;
			string dirstring= ConfigManager::gamedataPath+string("characters/")+value;

			dir = opendir(dirstring.c_str());

			vector <string> fileNames;

			printf("\t\t- Animations Dir: %s\n",dirstring.c_str());
			while((ep=readdir(dir))!=NULL)
			{
				bool isAConfFile = string(ep->d_name).find("cfg",1) !=string::npos;

				if(ep->d_name[0]!='.' && isAConfFile)
				{
					string fileString = dirstring + string("/") + string(ep->d_name);
					fileNames.push_back(fileString);
				}
			}
            
            vector<string>::iterator it;
            for(it=fileNames.begin();it!=fileNames.end();it++)
            {
				printf("\t\t\t- Loading animation: %s\n", (*it).c_str());
				CharacterAction * act =	LoadAction(*it);
				std::pair<string, CharacterAction *>  * mypair = new pair<string,CharacterAction *>(act->actionName,act);
				
				charData->actions.insert(*mypair);
			}
		}
		else if (name == "sizex"){
			charData->sizex = atoi(value.c_str());
		}
		else if (name == "sizey"){
			charData->sizey = atoi(value.c_str());
		}
		else if (name == "updateTime"){
			charData->updateTime = atoi(value.c_str());
		}
		else if (name == "characterName"){
			charData->characterName = value;
		}	
		else if (name == "commonYOffset"){
			charData->commonYOffset = atoi(value.c_str());
		}
		else if (name == "commonXOffset"){
			charData->commonXOffset = atoi(value.c_str());
		}
		
	}
	configFile.close();

	std::pair<string, CharacterData *>  * mypair = new pair<string,CharacterData *>(charData->characterName,charData);        
	characterTypes.insert(*mypair);
	return charData;
}

CharacterAction * CharacterBuilder::LoadAction(string fileName)
{
	std::ifstream configFile (fileName.c_str());

	CharacterAction * action = new CharacterAction;
	action->imagesLoaded = false;
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
		
		if(name == "actionName"){
			action->actionName = value;
		}
		else if (name == "actionAnimationPath"){
			action->actionAnimationPath = value;
			action->animationImages = new ListaImagenes();
			action->animationImages->CargarDirectorio(value.c_str());
			action->numberOfFrames = action->animationImages->GetSize();
			action->imagesLoaded = true;
		}
		else if (name == "doesFire"){
			action->doesFire = (value == "true");
		}	
		else if (name == "doesMeleeAttack"){
			action->doesMeleeAttack = (value == "true");
		}
		else if (name == "doesJump"){
			action->doesJump = (value == "true");
		}
		else if (name == "doesDie"){
			action->doesDie = (value == "true");
		}
		else if (name == "doesSound"){
			action->doesSound = (value == "true");
		}
		else if (name == "bboxXOffset"){
			action->bboxXOffset = atoi(value.c_str());
		}
		else if (name == "bboxYOffset"){
			action->bboxYOffset = atoi(value.c_str());
		}
		else if (name == "isFacingRight"){
			action->isFacingRight = (value == "true");
		}
		else if (name == "frameToFire"){
			action->frameToFire = atoi(value.c_str());
		}
		else if (name == "frameToMelee"){
			action->frameToMelee = atoi(value.c_str());
		}
		else if (name == "frameToJump"){
			action->frameToJump = atoi(value.c_str());
		}
		else if (name == "frameToDie"){
			action->frameToDie = atoi(value.c_str());
		}
		else if (name == "frameToSound"){
			action->frameToSound = atoi(value.c_str());
		}
		else if (name == "shotBullets"){
			action->shotBullets = atoi(value.c_str());
		}
		else if (name == "dispersionX"){
			action->dispersionX = (float) atof(value.c_str());
		}
		else if (name == "dispersionY"){
			action->dispersionY = (float) atof(value.c_str());
		}
		else if (name == "jumpForce"){
			action->jumpForce = atoi(value.c_str());
		}
		else if (name == "accelX"){
			action->accelX = atoi(value.c_str());
		}
		else if (name == "fileToSound"){
			action->fileToSound = value;
		}
		else if (name == "probFaceToFace"){
			action->probFaceToFace = (float) atof(value.c_str());
		}
		else if (name == "probNoContact"){
			action->probNoContact = (float) atof(value.c_str());
		}
		else if (name == "probShotRange"){
			action->probShotRange = (float) atof(value.c_str());
		}
		else if (name == "probJumpableWall"){
			action->probJumpableWall = (float) atof(value.c_str());
		}
		else if (name == "probUnjumpableWall"){
			action->probUnjumpableWall = (float) atof(value.c_str());
		}
	}
	configFile.close();
	return action;
}
