#include "tile.h"

map<string,TileData *> TileBuilder::tileTypes;

Tile::Tile (string tileType, float integrity)
{
	this->tileType = tileType;
    this->integrity = integrity;
}

Tile::Tile (string tileType, int integrity)
{
    this->tileType = tileType;
    this->integrity= integrity * TileBuilder::GetTileDataByName(tileType)->integrityPerLevel;
}

bool Tile::disminuirNivelDeIntegridad (void)    //devuelve true si est· reventao
{
	// non-solid tiles don't interact with the rest of the world.
    if (!TileBuilder::GetTileDataByName(tileType)->isSolid) return (false);   
    
    //if the tile cannot be broken, it's left as it is
	if(integrity>TileBuilder::GetTileDataByName(tileType)->maxBreakableIntegrity) return(false); 

	integrity -=1.0f;
    
	if (integrity <= 0)
	{
		integrity = -1;
		return (true);
	}
	return (false);
}

bool Tile::disminuirNivelDeIntegridad(float damage) //devuelve true si se revienta el tile
{
	// non-solid tiles don't interact with the rest of the world
    if (!TileBuilder::GetTileDataByName(tileType)->isSolid) return (false);
	 
    //if the tile cannot be broken, it's left as it is
    if(integrity>TileBuilder::GetTileDataByName(tileType)->maxBreakableIntegrity) return(false);  
	integrity-=damage;
    
	if (integrity <= 0)
	{
		integrity = -1;
		return (true);
	}
	return (false);
}

SDL_Surface *Tile::GetImagen (void)
{   
    int imagesNumber = TileBuilder::GetTileDataByName(tileType)->tileImages->lista.size();
    float integrityPerLevel = TileBuilder::GetTileDataByName(tileType)->integrityPerLevel;
    return (TileBuilder::GetTileDataByName(tileType)->tileImages->GetImagen((int)(imagesNumber-integrity/integrityPerLevel)));
}


float Tile::GetIntegrity(void)
{
	return (integrity);
}

int Tile::GetIntegrityLevel()
{
    float integrityPerLevel = TileBuilder::GetTileDataByName(tileType)->integrityPerLevel;
	return ((int)ceil(integrity/integrityPerLevel));
}

void Tile::SetIntegrity(float integrity)
{
	this->integrity=integrity;
}

string Tile::GetType(void)
{
	return (tileType);
}

void Tile::SetType(string type)
{
	this->tileType=type;
}

bool Tile::esTraspasable(void)
{
	return (!TileBuilder::GetTileDataByName(tileType)->isSolid);
}

void Tile::CrearExplosiones(int x, int y)
{
        ListaImagenes * imagenesDissolve = TileBuilder::GetTileDataByName(tileType)->dissolveAnimation;
	new Animacion(
                    x+DIMENSION_TILE/2,
                    y+DIMENSION_TILE-imagenesDissolve->AltoImagenes()/2,
                    imagenesDissolve
                    );	
}

void TileBuilder::LoadTileTypes()
{
        struct dirent *ep=NULL;
        DIR * dir;
        string dirstring= ConfigManager::gamedataPath + string("tiles/");

        dir = opendir(dirstring.c_str());

        while((ep=readdir(dir))!=NULL)
        {
                bool isAConfFile = string(ep->d_name).find("cfg",1) !=string::npos;

                if(ep->d_name[0]!='.' && isAConfFile)
                {
                        string fileString = dirstring+string(ep->d_name);
                        TileData * tileData = TileBuilder::LoadTileData(fileString);
                        if(tileData->isEndLevelPoint)
                                printf("Nombre del endlevel: %s\n",tileData->tileName.c_str());
                        std::pair<string,TileData *> * tilePair = new std::pair<string, TileData *>(tileData->tileName,tileData);
                        TileBuilder::tileTypes.insert(*tilePair);
                }
        }
}

TileData * TileBuilder::LoadTileData(string filename)
{
        std::ifstream configFile (filename.c_str());

        TileData * tileData = new TileData;
        printf("\t- New tile from: %s\n",filename.c_str());
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
                if (name == "tileName"){
                        tileData->tileName = value;
                }
                else if (name == "isSolid"){
                        tileData->isSolid = (value=="true");
                }
                else if (name == "imagesDir"){
                        tileData->imagesDir = value;
                        tileData->tileImages = new ListaImagenes();
                        tileData->tileImages->CargarDirectorio((string("tiles/")+value).c_str());
                }
                else if (name == "maxBreakableIntegrity"){
                        tileData->maxBreakableIntegrity = (float) atof(value.c_str());
                }
                else if (name == "dissolveAnimDir"){
                        tileData->dissolveAnimDir = value;
                        tileData->dissolveAnimation = new ListaImagenes();
                        tileData->dissolveAnimation->CargarDirectorio((string("tiles/")+value).c_str());
                }
                else if (name == "doesIncreaseHealth"){
                        tileData->doesIncreaseHealth = (value == "true");
                }
                else if (name == "doesIncreaseAmmo"){
                        tileData->doesIncreaseAmmo = (value == "true");
                }
                else if (name == "doesIncreaseFirepower"){
                        tileData->doesIncreaseFirepower = (value == "true");
                }
                else if (name == "doesIncreasePlayerSpeed"){
                        tileData->doesIncreasePlayerSpeed = (value == "true");
                }
                else if (name == "isEndLevelPoint"){
                        tileData->isEndLevelPoint = (value == "true");
                }
                else if (name == "isStartLevelPoint"){
                        tileData->isStartLevelPoint = (value == "true");
                }
                else if (name == "integrityPerLevel"){
                        tileData->integrityPerLevel = (float) atof(value.c_str());
                }
        }
        return tileData;                
}

TileData * TileBuilder::GetTileDataByName(string tileName)
{
        return TileBuilder::tileTypes[tileName];
}
