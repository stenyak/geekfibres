
#include "editor.h"

static Palette * editorPalette;

void Editor ( SDL_Surface * screen )
{
	bool finPartida = false;
	int posCamaraX = ConfigManager::H_RES / 2; //posiciÛn actual de la c·mara de scrolling !! indica el centro de la pantalla !!
	int posCamaraY = ConfigManager::V_RES / 2;
    
	//cargamos el cursor que se mover· con el teclado    
	SDL_Surface *tempImage = IMG_Load ( (ConfigManager::gamedataPath + string ("cursor.png" )).c_str());
	int colorKey = SDL_MapRGB ( tempImage->format, 248, 248, 232 );
    
	SDL_SetColorKey ( tempImage, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorKey );
	SDL_Surface *imagenCursor = SDL_DisplayFormat ( tempImage );
	SDL_SetAlpha ( imagenCursor, SDL_SRCALPHA | SDL_RLEACCEL, 128 );    //el cursor es semitransparente
    
	delete tempImage;
	SDL_Rect rectCursor;
    
	rectCursor.h = imagenCursor->h;
	rectCursor.w = imagenCursor->w;
    
	int posCursorX = DIMENSION_TILE / 2;
	int posCursorY = 600 - DIMENSION_TILE / 2;
    
	// inicializar todo
	SDL_ShowCursor ( SDL_ENABLE );
    
	Mundo mundoActual;
	LevelData * levelData = new LevelData(ConfigManager::gamedataPath + string ("editor.cfg") );
        
	mundoActual.CargarMundo (levelData);
    
	editorPalette = new Palette(levelData);
    
	Uint32 tiempoInicioBucle;   //indica cu·ndo comenzÛ el bucle actual
    
	// 25 fps -> 40ms/frame Indica el tiempo m·ximo entre bucle y bucle.
	const Uint32 tiempoPorBucle = 1000 / 25;
    
	//-----------Main Loop-----------
	while ( !finPartida )
	{
		tiempoInicioBucle = SDL_GetTicks (  );
		SDL_PumpEvents (  );
		editorPalette->ProcessEvents();
		GestionarEventosEditor ( posCursorX, posCursorY, finPartida,mundoActual,posCamaraX, posCamaraY);
        
		//posCamaraX += int ( float ( posCursorX - posCamaraX ) / factInercia );
		//posCamaraY += int ( float ( posCursorY - posCamaraY ) / factInercia );
        
		// limpiamos la pantalla antes de dibujar los tiles
		SDL_FillRect ( screen, NULL, 0 );
        
		mundoActual.PintaMundo ( posCamaraX, posCamaraY, screen );
		
		// posicionamos el cursor
		rectCursor.x = posCursorX - posCamaraX + ConfigManager::H_RES / 2 - 32;
		rectCursor.y = -posCursorY + posCamaraY + ConfigManager::V_RES / 2 - 8;

		if(rectCursor.x > ConfigManager::H_RES - 2 * DIMENSION_TILE)
			posCamaraX += (posCursorX - posCamaraX + DIMENSION_TILE)/10;
		if(rectCursor.y > ConfigManager::V_RES - 2 * DIMENSION_TILE )
			posCamaraY += (posCursorY-posCamaraY + DIMENSION_TILE)/10;
		if(rectCursor.x < DIMENSION_TILE )
			posCamaraX -= (posCamaraX-posCursorX)/10;
		if(rectCursor.y < DIMENSION_TILE )
			posCamaraY -= (posCamaraY-posCursorY)/10;

		// pintamos el cursor
		SDL_BlitSurface ( imagenCursor, NULL, screen, &rectCursor );
		editorPalette->Draw(screen);

		// volcamos todo a pantalla
		SDL_Flip ( screen );

		// Wait some time until the next frame must be drawn
		while ( SDL_GetTicks (  ) - tiempoInicioBucle < tiempoPorBucle )
			SDL_Delay(tiempoPorBucle - SDL_GetTicks() + tiempoInicioBucle);
	}

	delete editorPalette;
	editorPalette=NULL;
	SDL_ShowCursor ( SDL_DISABLE );

	SDL_Event evento;

	while ( SDL_PollEvent ( &evento ) ) ;   // esto es para evitar un bug en el men˙
	// FIXME lo logico seria hacer esto en el propio menu
}

void GestionarEventosEditor ( int &posCursorX, int &posCursorY,
		bool & finPartida, Mundo & mundoActual, int & posCamaraX, int & posCamaraY)
{
	int tileX = posCursorX / DIMENSION_TILE;
	int tileY = posCursorY / DIMENSION_TILE;
	SDL_Event * ev = new SDL_Event;

	static int mousePosX, mousePosY, lastMousePosX, lastMousePosY;
	Uint8 buttonsState = SDL_GetMouseState(&mousePosX, &mousePosY);

	Uint8 *teclasPulsadas = SDL_GetKeyState ( NULL );

	while ( SDL_PollEvent ( ev ) )
	{
		if(ev->type == SDL_MOUSEMOTION || ev->type == SDL_MOUSEBUTTONDOWN)
		{
			if(buttonsState & SDL_BUTTON_LMASK) // Mouse left button is pressed
			{
				// change from screen coordinates to world coordinates
				int worldX = ((mousePosX - ConfigManager::H_RES/2 )+posCamaraX)/DIMENSION_TILE;
				int worldY = (posCamaraY - (mousePosY - ConfigManager::V_RES/2 ) )/DIMENSION_TILE;

				// avoid getting out of the array
				if (worldX >= ANCHO_MUNDO || worldY >= ALTO_MUNDO || worldX<0 || worldY<0)
					break;

				posCursorX = DIMENSION_TILE * worldX + DIMENSION_TILE/2;
				posCursorY = DIMENSION_TILE * worldY + DIMENSION_TILE*9/10-1;

				// write that tile
				ThumbData * td = editorPalette->GetCurrentTileType();

				if(mundoActual.tilesMundo[worldX][worldY]!=NULL)
					delete mundoActual.tilesMundo[worldX][worldY];

				if(td==NULL || 	teclasPulsadas[SDLK_LSHIFT] )
					mundoActual.tilesMundo[worldX][worldY] = NULL;
				else
					mundoActual.tilesMundo[worldX][worldY] = 
						new Tile(td->tileName,td->integrityLevel);
			}
			else if(buttonsState & SDL_BUTTON_RMASK) // Mouse right button is pressed
			{
				if(lastMousePosX != -1)
				{
					posCamaraX += lastMousePosX - mousePosX;
					posCamaraY -= lastMousePosY - mousePosY;

					// change from screen coordinates to world coordinates
					int worldX = ((mousePosX - ConfigManager::H_RES/2 )+posCamaraX)/DIMENSION_TILE;
					int worldY = (posCamaraY - (mousePosY - ConfigManager::V_RES/2 ) )/DIMENSION_TILE;

					if(worldX<0)
						worldX=0;
					if(worldY<0)
						worldY=0;
					if(worldX>=ANCHO_MUNDO)
						worldX=ANCHO_MUNDO-1;
					if(worldY>=ALTO_MUNDO)
						worldY=ALTO_MUNDO-1;

					// we move the cursor with us so as to be able to move the
					// viewport with the mouse, without it bouncing back to its
					// previous position
					posCursorX = DIMENSION_TILE * worldX + DIMENSION_TILE/2;
					posCursorY = DIMENSION_TILE * worldY + DIMENSION_TILE*9/10-1;
				}
			}
			else
			{
				lastMousePosX = -1;
				lastMousePosY = -1;
			}
		}
	}

	if ( teclasPulsadas[SDLK_UP] )
		if ( posCursorY / DIMENSION_TILE < ALTO_MUNDO - 1 )
			posCursorY += DIMENSION_TILE;

	if ( teclasPulsadas[SDLK_DOWN] )
		if ( posCursorY / DIMENSION_TILE > 0 )
			posCursorY -= DIMENSION_TILE;

	if ( teclasPulsadas[SDLK_LEFT] )
		if ( posCursorX / DIMENSION_TILE > 0 )
			posCursorX -= DIMENSION_TILE;

	if ( teclasPulsadas[SDLK_RIGHT] )
		if ( posCursorX / DIMENSION_TILE < ANCHO_MUNDO - 1 )
			posCursorX += DIMENSION_TILE;

	if ( teclasPulsadas[SDLK_SPACE] )
	{
		ThumbData * td = editorPalette->GetCurrentTileType();
		if(mundoActual.tilesMundo[tileX][tileY]!=NULL)
			delete mundoActual.tilesMundo[tileX][tileY];
		if(td==NULL)
			mundoActual.tilesMundo[tileX][tileY] = NULL;
		else
			mundoActual.tilesMundo[tileX][tileY]=new Tile(td->tileName,td->integrityLevel);
	}   
	if(teclasPulsadas[SDLK_LSHIFT] )
	{
		if(mundoActual.tilesMundo[tileX][tileY]!=NULL)
			delete mundoActual.tilesMundo[tileX][tileY];

		mundoActual.tilesMundo[tileX][tileY] = NULL;
	}

	if ( teclasPulsadas[SDLK_F1] )
		mundoActual.GuardarMundo ( 
				(ConfigManager::gamedataPath + string ("useredit.dat")).c_str() );

	if ( teclasPulsadas[SDLK_ESCAPE] )  // provocar la salida del juego
		finPartida = true;

	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;
}
