/******************************************************************************\
 *                                   Juego                              
 *  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * …sta funcion es el "main loop" del juego en sÌ. La funciÛn espera que todos
 * los datos del mundo y de los malos estÈn cargados en memoria cuando se le
 * llama.
 * 
 * La funciÛn Èsta, se encarga de controlar los eventos de teclado, de
 * actualizar las posiciones de todos los objetos que aparezcan en la pantalla,
 * y de renderizarlos.
 * 
 * Retorna el estado en el que se sale de la pantalla
 * 
 * 1:muerto, 2:fin de nivel, 3: abortado
 * 
 * Nota: espera que los datos ya estÈn cargados, para dar la posiblidad de que
 * se pueda cargar una partida de disco, o de iniciar una partida nueva. De Èso
 * se encargar·n otras funciones
 */
#include "juego.h"

int Juego ( SDL_Surface * screen, LevelData * level )
{
	int finPartida = 0;     
	/*  0 -> no hay fin
	 * 	1 -> vida acabada
	 * 	2 -> nivel acabado
	 * 	3 -> peticion de usuario
	 */

    float posCamaraX = 0;  // posiciÛn actual de la c·mara de scrolling
    float posCamaraY = 0;  // indica el centro de la pantalla
	
	Uint32 tiempoInicioBucle;   //indica cu·ndo comenzÛ el bucle actual
	const Uint32 tiempoPorBucle = 1000/ 20; //25 fps -> 40ms. Indica el tiempo m·ximo entre bucle y bucle.

    // Create Main Character
	Protagonista * prota = NULL;
	
    prota = new Protagonista(&finPartida,level->mainCharacter);
	
    // Load Level
	Mundo mundoActual;
	mundoActual.CargarMundoProta ( level, prota );

    // Load Background
	SDL_Surface *imagenFondoParallax;
	SDL_Surface *tempImage = IMG_Load((ConfigManager::gamedataPath + level->GetBackgroundFilename()).c_str());

	imagenFondoParallax = SDL_DisplayFormat ( tempImage );
	delete tempImage;
	
    // Start music
	bool mustPlayMusic = ConfigManager::GetBoolProperty("mustPlayMusic");
	if(mustPlayMusic)	
		MusicSystem::PlayMusic();
	
	//-----------Main Loop-----------
	// fin de *partida* ? se supone que tenemos que poder volver al menu para
	// guardar sin que se nos vaya pique la partida (sin que se finalize la Partida) (bySTK)

	while ( !finPartida )
	{
        tiempoInicioBucle = SDL_GetTicks();
		
		GestionarEventosDeEntrada ( prota, finPartida, screen, &mustPlayMusic );

		ActualizarEstadoDelMundo ( prota, mundoActual, posCamaraX, posCamaraY );

		int posCamaraXi = int(floor(posCamaraX));
		int posCamaraYi = int(floor(posCamaraY));

		// We render everything
		DrawBackground(posCamaraXi, posCamaraYi, imagenFondoParallax, screen);

		mundoActual.PintaMundo  ( posCamaraXi, posCamaraYi, screen );
		Disparo::PintaTodos     ( posCamaraXi, posCamaraYi, screen );
		Malo::PintaTodos        ( posCamaraXi, posCamaraYi, screen );
		Animacion::PintaTodos   ( posCamaraXi, posCamaraYi, screen );
		prota->Pintate          ( posCamaraXi, posCamaraYi, screen );

		if (mustPlayMusic && ! Mix_PlayingMusic ( ) )
			MusicSystem::PlayMusic();
	
		SDL_Flip ( screen );

        // Wait some time until the next frame must be drawn
    	while ( SDL_GetTicks (  ) - tiempoInicioBucle < tiempoPorBucle )
    		SDL_Delay(tiempoPorBucle - SDL_GetTicks() + tiempoInicioBucle);
        
	}
	if ( Mix_PlayingMusic () )
		MusicSystem::StopMusic ();
	Disparo::MataTodos ();
	Malo::MataTodos();
	SDL_FreeSurface ( imagenFondoParallax );
	return ( finPartida );
}

//
// This function draws the game backround, moving it around slowly following
// the camera movements
// FIXME: OPTIMIZE THIS METHOD NOW!!!!
void DrawBackground(int posCamaraXi, int posCamaraYi, SDL_Surface * imagenFondoParallax, SDL_Surface * screen)
{
	static SDL_Rect *rectFondoParallax = new SDL_Rect;
	rectFondoParallax->w = imagenFondoParallax->w;
	rectFondoParallax->h = imagenFondoParallax->h;

    static int numTilesX = (int)ceil(((float)(ConfigManager::H_RES)) / imagenFondoParallax->w)+1;
	static int numTilesY = (int)ceil(((float)(ConfigManager::V_RES)) / imagenFondoParallax->h)+1;
	
	for(int i=-1;i<numTilesY;i++)
		for(int j=0;j<numTilesX;j++)
		{
			rectFondoParallax->x = j*imagenFondoParallax->w - (posCamaraXi/3) % imagenFondoParallax->w;
			rectFondoParallax->y = i*imagenFondoParallax->h + (posCamaraYi/3) % imagenFondoParallax->h - ConfigManager::V_RES;
			if(rectFondoParallax->x>(-imagenFondoParallax->w) && 
               rectFondoParallax->x<ConfigManager::H_RES && 
               rectFondoParallax->y>(-imagenFondoParallax->h) && 
               rectFondoParallax->y < ConfigManager::V_RES)
                SDL_BlitSurface ( imagenFondoParallax, NULL, screen, rectFondoParallax );
		}
}

void GestionarEventosDeEntrada ( Protagonista * prota,
		int &finPartida, SDL_Surface * screen , bool * mustPlayMusic)
{
	SDL_Event evento;
	static bool balasUp=0; //indica si las balas deben tener "efecto" hacia arriba o hacia abajo
	static bool balasDown=0;

	while ( SDL_PollEvent ( &evento ) )
	{
		switch ( evento.type )
		{
			case SDL_KEYDOWN:
				switch ( evento.key.keysym.sym )
				{
					case SDLK_m:       //alterna entre musica ON y musica OFF
                        *mustPlayMusic = !*mustPlayMusic;
						Mix_PlayingMusic (  )? MusicSystem::StopMusic() :
							MusicSystem::PlayMusic();
						break;
					
                    case SDLK_PRINT:
						SDL_SaveBMP ( screen, "screenshot.bmp" );
						break;
					
                    case SDLK_LSHIFT:
						prota->PleaseJump (  );
						break;
					
                    case SDLK_DOWN:    // pedir al prota que se agache
						balasDown=true;
						break;
					
                    case SDLK_UP:
						balasUp=true;
						break;

					case SDLK_RCTRL:
					case SDLK_LCTRL:
						Disparo::DisparoMultiple ( prota->GetPosX (  ) + 60, // FIXME: that hardcoded position isn't even correct
								prota->GetPosY (  ) + 70,
								prota->GetLadoAndar (  )? rand()%100/10.0+30 : -rand()%100/10.0-30,
								4*(int)balasUp-4*(int)balasDown,
								true,
								75,
								5.0f,
								6.1f
								);
						break;

                    case SDLK_c:
						Disparo::DisparoMultiple ( prota->GetPosX (  ) + 60, 
							 prota->GetPosY (  ) + 70,
							 prota->GetLadoAndar (  )? 35 : -35,
							 4*(int)balasUp-4*(int)balasDown,
							 true,
							 250,
							 5.1f,
							 15.1f
							 );
							 break;
                             
					case SDLK_LEFT:    // pedir al prota que avance a izquiera
						prota->PleaseGoLeft (  );
						break;
                        
					case SDLK_RIGHT:   // pedir al prota que avance a derecha
						prota->PleaseGoRight (  );
						break;
                        
					case SDLK_ESCAPE:  // provocar la salida del juego
						finPartida = 3;
						break;
						
						// hacks para debuggin!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					case SDLK_q:
						prota->hackDesplazate ( 0, +100 );
						break;
					case SDLK_a:
						prota->hackDesplazate ( 0, -100 );
						break;
					case SDLK_o:
						prota->hackDesplazate ( -50, 0 );
						break;
					case SDLK_p:
						prota->hackDesplazate ( +50, 0 );
						break;
					case SDLK_z:
						prota->hackDesplazate ( -100, 0 );
						break;
					case SDLK_x:
						prota->hackDesplazate ( +100, 0 );
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				switch ( evento.key.keysym.sym )
				{
					case SDLK_DOWN:
						balasDown=false;
						break;
                        
					case SDLK_UP:
						balasUp=false;
						break;
                        
					case SDLK_LEFT:    // ask the main character to stop moving to the left
						prota->PleaseStopLeft (  );
						break;
                        
					case SDLK_RIGHT:   // ask the main character to stop moving to the right
						prota->PleaseStopRight (  );
						break;
                        
					default:
						break;
				}
			default:
				break;
		}
	}                       
}

