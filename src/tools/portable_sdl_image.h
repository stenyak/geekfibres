#ifndef PORTABLE_SDL_IMAGE_H
    #define PORTABLE_SDL_IMAGE_H

    #ifdef WIN32
        #include "sdl_image.h"
    #else 
        #ifdef __APPLE__ 
            #include <SDL_image/SDL_image.h>
        #else
            #include <SDL/SDL_image.h>
        #endif
    #endif
#endif
