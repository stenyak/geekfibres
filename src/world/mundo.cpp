#include "../world/mundo.h"

Mundo::~Mundo()
{
	for(int x=0; x<ANCHO_MUNDO;x++)
		for(int y=0;y<ALTO_MUNDO;y++)
			if(tilesMundo[x][y]!=NULL)
				delete tilesMundo[x][y];
}

/***************************************************************\
*                         CargaMundo	                        *
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                                               *
* Crea el mundo en memoria a partir del archivo que se pasa	*
* como par·metro.						*
* Queda por definir cÛmo se cargar·n los malos... Se podrÌa 	*
* hacer una segunda pasada al archivo, y en ella cargar a los 	*
* malos...  :S
\***************************************************************/

void Mundo::CargarMundo (LevelData * level)
{
    CargarMundoProta (level, NULL);
}

void Mundo::CargarMundoProta (LevelData * level, Protagonista * prota)
{
    FILE *archivoMundo = fopen ((ConfigManager::gamedataPath + level->GetLevelFilename()).c_str(), "r");
    this->levelData = level;

    char dato,dato2;                            // Last data read from disc
    int coordx = 0, coordy = ALTO_MUNDO - 1;    //coordenadas (x,y) del array bidimensional de

    // We initialize all the world to NULL
    for (int x = 0; x < ANCHO_MUNDO; x++)
        for (int y = 0; y < ALTO_MUNDO; y++)
        {
            tilesMundo[x][y] = NULL;
        }
    
    while (fread (&dato, 1, 1, archivoMundo))
    {
        if(dato =='\n'){
            coordy--;
            coordx=0;
            continue;
        }               
        fread (&dato2, 1, 1, archivoMundo);
       
        if(dato!=' '){
            if(level->storedTileChars[dato]!=string(""))
            { 
                // it's a tile
                char dataString[2]={dato2,'\0'};
                int integrity = atoi(dataString);
                tilesMundo[coordx][coordy] = new Tile(level->storedTileChars[dato],integrity);
            }
            else if(level->storedCharacterChars[dato]!=string(""))
            {
                // it's a character
                if(level->mainCharacter->characterName == level->storedCharacterChars[dato])
                {
                    if (prota)
                    {
                        prota->SetPosVelX(coordx * DIMENSION_TILE, 0);
                        prota->SetPosVelY(coordy * DIMENSION_TILE, 0);
                    }
                }
                else
                {       
                    new Malo(coordx * DIMENSION_TILE, coordy * DIMENSION_TILE, CharacterBuilder::GetCharacterByName("momia"));
                }
           }
        }
      coordx++;
    }
    fclose (archivoMundo);
}


void Mundo::PintaMundo (int posCamaraX, int posCamaraY, SDL_Surface * screen)
{
    SDL_Rect *rectanguloTemporal;

    for (int i = 0; i < ANCHO_MUNDO; i++)   //ATENCI”N, NO OPTIMIZADO, tendrÌa que
    {
        for (int j = 0; j < ALTO_MUNDO; j++)  
        {
            if ((((i + 1) * DIMENSION_TILE) > posCamaraX - ConfigManager::H_RES/2) && ((i * DIMENSION_TILE) < posCamaraX + ConfigManager::H_RES/2)
                && (tilesMundo[i][j] && (tilesMundo[i][j]->GetImagen ())))
            {
                rectanguloTemporal = new SDL_Rect;
                rectanguloTemporal->x = (i * DIMENSION_TILE) - (posCamaraX - ConfigManager::H_RES/2);
                rectanguloTemporal->y = (posCamaraY + ConfigManager::V_RES/2) - (j * DIMENSION_TILE + DIMENSION_TILE);
                rectanguloTemporal->h = rectanguloTemporal->w = DIMENSION_TILE;
                SDL_BlitSurface (tilesMundo[i][j]->GetImagen (), NULL, screen, rectanguloTemporal);
                delete rectanguloTemporal;
            }
        }
    }

}

char Mundo::SearchStringInStoredCharactersTileMap(string tileName)
{
        for(int i=0;i<255;i++)
        {
            if(levelData->storedTileChars[(char)i]==tileName)
                return i;    
        }
        return ' ';
}

void Mundo::GuardarMundo(const char *nombreArchivoMundo)
{
    FILE *archivoMundo = fopen (nombreArchivoMundo, "w");

    // mundoActual
    //inicializamos el mundo con casillas vacias
    for (int y = ALTO_MUNDO-1 ; y >=0; y--)
    {
	    for (int x = 0; x < ANCHO_MUNDO; x++)
        {
	        if(tilesMundo[x][y]==NULL)
            {
                fputc(' ',archivoMundo);
		        fputc(' ',archivoMundo);
            }
	        else
	        {
                string type=tilesMundo[x][y]->GetType();
                int integrityLevel=tilesMundo[x][y]->GetIntegrityLevel();
                char integrityString [20];
                sprintf(integrityString,"%i",integrityLevel);
                    
                char c = SearchStringInStoredCharactersTileMap(type);
                fputc(c,archivoMundo);
                integrityString[0] = '0'+ integrityLevel;
                fputc(integrityString[0],archivoMundo);
    	    }
        }
	    if(y!=ALTO_MUNDO)
		    fputc('\n',archivoMundo);
    }
    fclose (archivoMundo);
}


