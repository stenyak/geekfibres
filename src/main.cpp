#include "main.h"

SDL_Surface *screen;

enum menu_states {ENTER_GAME, ENTER_EDITOR, EXIT} ;

enum game_states {GAME_OVER=0, USER_EXIT=1, LEVEL_FINNISHED=2};


int main (int argc, char **argv)
{
    int accionARealizar;        //0:juego,1:cargar,2:salir
    int pantallaActual = 0;
    int retornoJuego;
    printf("starting shit");

    ConfigManager::LoadConfigFile("config.cfg");

    if (! InicializarSDL())
    {
        printf("\nfuck this shit, i'm out");
        return 0;
    }
    printf("\nall is ok man");
    CharacterBuilder::LoadCharacterTypes();
    LevelData::LoadLevels();
    TileBuilder::LoadTileTypes();
    printf("weeee i'm a loop!");

    while ( ( accionARealizar = Menu (  ) ) != EXIT )
    {
        switch ( accionARealizar )
        {
        case ENTER_GAME:
            retornoJuego = LEVEL_FINNISHED;
            while ( retornoJuego == LEVEL_FINNISHED )
            {
                retornoJuego = Juego ( screen, LevelData::GetLevelData(pantallaActual));
                switch ( retornoJuego )
                {
                case GAME_OVER: // muerto
                    mostrarMenu ( (ConfigManager::gamedataPath+string("gameover.jpg")).c_str() );
                    pantallaActual = 0;
                    break;
                case LEVEL_FINNISHED: // fin de nivel
                    if (++pantallaActual == LevelData::GetNumberOfLevelsLoaded())  //has acabao el juego
                    {
                        mostrarMenu ( (ConfigManager::gamedataPath+string("gamecompleted.jpg")).c_str() );
                        mostrarMenu ( (ConfigManager::gamedataPath+string("creditos.jpg")).c_str() );
                        pantallaActual = 0;
                        retornoJuego = GAME_OVER;
                    }else{
                        mostrarMenu ( (ConfigManager::gamedataPath+string("levelcompleted.jpg")).c_str() );
                    }
                    break;
                case USER_EXIT:default: // peticion de user
                    retornoJuego = USER_EXIT;
                    break;
                }
            }
            break;
        case ENTER_EDITOR:
            Editor ( screen );
            //^--Ahora mismo vuelve a ejecutar el men˙
            break;
        }
    }
    printf("out of loop :-(");

    SDL_Quit (  );
    return 0;
}

/***************************************************************\
*                         InicializarSDL                        *
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *  
*                                                               *
*  Inicializa todos los subsistemas de SDL, y crea la ventana.  *
*  Devuelve 0 si hay alg˙n error. 1 otherwise                   *
*                                                               *
\***************************************************************/

bool InicializarSDL (  )
{
    //inicializamos el video
    if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
	    return false;       
    }
    if(ConfigManager::GetBoolProperty("fullscreenWindow"))
    {    
	    if ( !( screen = SDL_SetVideoMode ( 
					    ConfigManager::H_RES, ConfigManager::V_RES, 0, 
					    SDL_HWSURFACE | SDL_DOUBLEBUF | 
					    SDL_HWACCEL  | SDL_FULLSCREEN) ) )
	    {
		    return false;
	    }
    }
    else
    {
	    if ( !( screen = SDL_SetVideoMode ( 
					    ConfigManager::H_RES, ConfigManager::V_RES, 0, 
					    SDL_HWSURFACE | SDL_DOUBLEBUF |
					    SDL_HWACCEL	) ) )
	    {
		    return false;
	    }

    }

    // Hacemos desaparecer el ratÛn
    SDL_ShowCursor ( SDL_DISABLE );
    
    // Inicializamos el sistema de audio
    int audioRate = 22050;
    Uint16 audioFormat = AUDIO_S16;
    int audioChannels = 2;
    int audioBuffer = 1024;

    if ( Mix_OpenAudio ( audioRate, audioFormat, audioChannels, audioBuffer ) )
    {
        printf ( "No se pudo abrir el sistema de sonido!\n" );
    }
    //Mix_QuerySpec ( &audioRate, &audioFormat, &audioChannels );
    printf("\nwhataer\n");
    //return false;

    // Inicializamos las fuentes de letras
    if ( TTF_Init (  ) == -1 )
    {
        printf( "No se pudo inicializar el sistema de fuentes de letras TTF\n" );
        return false;
    }
    return true;
}

/***************************************************************\
*                         Menu                                  *
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                                               *
*  Muestra y controla el men˙ del juego.                        *
*  Devuelve 	0 si hay que arrancar el juego			*
*  		1 si hay que cargar partida			*
*  		2 si hay que salir
\***************************************************************/
void mostrarMenu ( const char* imagenFondo )
{
    bool salir = false;
    SDL_Event evento;

    SDL_Surface * fondoMenu = IMG_Load(imagenFondo);

    SDL_Rect *rectFondoMenu = new SDL_Rect;

    rectFondoMenu->x = 0;
    rectFondoMenu->y = ConfigManager::V_RES - fondoMenu->h;
    rectFondoMenu->w = fondoMenu->w;
    rectFondoMenu->h = fondoMenu->h;

    while ( !salir )
    {
        // Hacemos el control del teclado
        while ( SDL_PollEvent ( &evento ) )
        {
            switch ( evento.type )
            {
            case SDL_KEYDOWN:
		if (evento.key.keysym.sym == SDLK_RETURN) salir = true;
                break;
            default:
                break;
            }
        }
        SDL_FillRect ( screen, NULL, ConfigManager::GetIntProperty("menuBackground") );  
	//SDL_MapRGB(screen->format, 0,0,int(50*sin(cuentaCiclos/40.0)+100)));
        SDL_BlitSurface ( fondoMenu, NULL, screen, rectFondoMenu );
        SDL_UpdateRect ( screen, 0, 0, 0, 0 );

    }
    SDL_FreeSurface ( fondoMenu );
    SDL_ShowCursor ( SDL_ENABLE );

}


int Menu (  )
{
    bool salir = false;
    SDL_Event evento;
    char opcion;                // 0:JUGAR, 1:CONTINUAR_PARTIDA, 2:SALIR

    // cargamos la imagen del puntero, y creamos la superficie y el SDL_Rect correspondientes
    SDL_Rect *posPuntero = new SDL_Rect;
    SDL_Surface *imagenPuntero;

    imagenPuntero = IMG_Load ( (ConfigManager::gamedataPath+string("punteromenu.png")).c_str() );
    posPuntero->h = imagenPuntero->h;
    posPuntero->w = imagenPuntero->w;
    posPuntero->x = screen->w / 2 - imagenPuntero->w / 2 + ConfigManager::H_RES / 4;
    SDL_SetColorKey ( imagenPuntero, SDL_SRCCOLORKEY,
                      SDL_MapRGB ( screen->format, 0, 0, 0 ) );

    // cargamos la imagen de fondo del men˙ y creamos la superficie correspondiente
    SDL_Surface * fondoMenu = IMG_Load( (ConfigManager::gamedataPath+string("mainmenu.jpg")).c_str());
    SDL_Rect *rectFondoMenu = new SDL_Rect;

    rectFondoMenu->x = ConfigManager::H_RES/2 - fondoMenu->w/2;
    rectFondoMenu->y = ConfigManager::V_RES/2 - fondoMenu->h/2;
    rectFondoMenu->w = fondoMenu->w;
    rectFondoMenu->h = fondoMenu->h;
    // realizamos todo el trabajo de las fuentes de letras TTF
    TTF_Font *fuenteMenu;
    
    if ( !( fuenteMenu = TTF_OpenFont ( (ConfigManager::gamedataPath+string("FreeSerifBold.ttf")).c_str(),42 ) ) )
    {
        printf ( "Error, no se pudo abrir la fuente de letras.\n" );
        exit ( 3 );
    }

    SDL_Color colorLetras;

    colorLetras.r = 255;
    colorLetras.g = 255;
    colorLetras.b = 255;
    colorLetras.unused=0;
    
    SDL_Rect *rectTitulo = new SDL_Rect;
    SDL_Rect *rectOpcion1 = new SDL_Rect;
    SDL_Rect *rectOpcion2 = new SDL_Rect;
    SDL_Rect *rectOpcion3 = new SDL_Rect;

    TTF_SetFontStyle ( fuenteMenu, TTF_STYLE_UNDERLINE );
    SDL_Surface *tituloMenu =
        TTF_RenderText_Solid ( fuenteMenu, "GeekFibres - Menú Principal", colorLetras );

    TTF_SetFontStyle ( fuenteMenu, TTF_STYLE_NORMAL );
    SDL_Surface *menuOpcion1 =
        TTF_RenderText_Solid ( fuenteMenu, "Empezar partida", colorLetras );
    SDL_Surface *menuOpcion2 =
        TTF_RenderText_Solid ( fuenteMenu, "Editar mundo", colorLetras );
    SDL_Surface *menuOpcion3 =
        TTF_RenderText_Solid ( fuenteMenu, "Salir", colorLetras );

    rectTitulo->h = tituloMenu->h;
    rectTitulo->w = tituloMenu->w;
    rectTitulo->x = ConfigManager::H_RES / 3;
    rectTitulo->y = ConfigManager::V_RES / 10;

    rectOpcion1->h = menuOpcion1->h;
    rectOpcion1->w = menuOpcion1->w;
    rectOpcion1->x = ConfigManager::H_RES / 2 - rectOpcion1->w / 2 + ConfigManager::H_RES / 4;
    rectOpcion1->y = int ( 2 * ConfigManager::V_RES / 7.0 );

    rectOpcion2->h = menuOpcion2->h;
    rectOpcion2->w = menuOpcion2->w;
    rectOpcion2->x = ConfigManager::H_RES / 2 - rectOpcion2->w / 2 + ConfigManager::H_RES / 4;
    rectOpcion2->y = int ( 3 * ConfigManager::V_RES / 7.0 );

    rectOpcion3->h = menuOpcion3->h;
    rectOpcion3->w = menuOpcion3->w;
    rectOpcion3->x = ConfigManager::H_RES / 2 - rectOpcion3->w / 2 + ConfigManager::H_RES / 4;
    rectOpcion3->y = int ( 4 * ConfigManager::V_RES / 7.0 );

    // bucle principal del menu
    opcion = 0;
    int cuentaCiclos = 0;

    const Uint32 tiempoPorBucle = 1000/50;
    Uint32 tiempoInicioBucle;
    while ( !salir )
    {       
        tiempoInicioBucle = SDL_GetTicks (  );
	    
        // Hacemos el control del teclado
        while ( SDL_PollEvent ( &evento ) )
        {
            switch ( evento.type )
            {
            case SDL_KEYDOWN:
                switch ( evento.key.keysym.sym )
                {
                case SDLK_UP:
                    opcion--;
                    break;

                case SDLK_DOWN:
                    opcion++;
                    break;

                case SDLK_ESCAPE:
                    opcion = 2;
                    salir = true;
                    break;

                case SDLK_RETURN:
                case SDLK_SPACE:
                    salir = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        // controlamos la salida del puntero de las opciones posibles
        if ( opcion < 0 ){
            opcion = 0;
        }
        if ( opcion > 2 ){
            opcion = 2;
        }

        // situamos el puntero seg˙n la opciÛn elegida
        posPuntero->y = int ( ConfigManager::V_RES / 7.0 * ( opcion + 2 ) + 10 );

        cuentaCiclos++;
        rectTitulo->x =
            int ( ConfigManager::H_RES / 2 + 30.0 * sin ( cuentaCiclos / 20.0 ) -
                  rectTitulo->w / 2 );

        // pintamos el men˙
        SDL_FillRect ( screen, NULL, ConfigManager::GetIntProperty("menuBackground") );  
        SDL_BlitSurface ( fondoMenu, NULL, screen, rectFondoMenu );
        SDL_BlitSurface ( imagenPuntero, NULL, screen, posPuntero );
        SDL_BlitSurface ( tituloMenu, NULL, screen, rectTitulo );
        SDL_BlitSurface ( menuOpcion1, NULL, screen, rectOpcion1 );
        SDL_BlitSurface ( menuOpcion2, NULL, screen, rectOpcion2 );
        SDL_BlitSurface ( menuOpcion3, NULL, screen, rectOpcion3 );

        SDL_UpdateRect ( screen, 0, 0, 0, 0 );
        
        while ( SDL_GetTicks (  ) - tiempoInicioBucle < tiempoPorBucle ){
            SDL_Delay(tiempoPorBucle - SDL_GetTicks() + tiempoInicioBucle);
        }
        
    }
    SDL_FreeSurface ( tituloMenu );
    SDL_FreeSurface ( menuOpcion1 );
    SDL_FreeSurface ( menuOpcion2 );
    SDL_FreeSurface ( menuOpcion3 );
    SDL_FreeSurface ( fondoMenu );
    SDL_FreeSurface ( imagenPuntero );
    delete rectOpcion1;
    delete rectOpcion2;
    delete rectOpcion3;
    delete rectTitulo;
    delete posPuntero;

    TTF_CloseFont ( fuenteMenu );
    return opcion;
}
