#ifndef PALETTE_H
#define PALETTE_H

#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include "../world/tile.h"
#include "../tools/portable_sdl.h"
#include "../tools/leveldata.h"

// This is the type used to store all the possible
// different tiles available
typedef struct
{
	string tileName;
	int integrityLevel;
} ThumbData;


// Type used to store the currently displayed tiles
// in the palette
typedef struct
{
    SDL_Rect rect;
    ThumbData * thumb;
} DisplayedTileData;

class Palette{
	private: 
        vector <SDL_Surface *> buttons; // the four buttons (back, back_pressed, forward,
                               // forward_pressed) are stored here
        SDL_Rect buttonRects[2];
		ThumbData * currentTileType;
        SDL_Rect borderPaletteRect; // size and coordinates of the border (will have a different color)
		SDL_Rect innerPaletteRect; 
		LevelData * levelData;
		static vector<ThumbData *> paletteThumbs;
        
        vector<DisplayedTileData *> tilesOnDisplay;
        
		int scrollAmount; // percentage of tiles scrolled at the moment
        static void addToVector(pair<string, TileData *> mypair);
        ThumbData * CollidedWithAnyThumbData(int x, int y);


		const static int horizontalWindowBorder=2;
		const static int upperWindowBorder = 25;
		const static int lowerWindowBorder = 3;
	public:
		Palette(LevelData * levelData);
		ThumbData * GetCurrentTileType();
		void Draw(SDL_Surface * screen);
		void ProcessEvents();
		void MoveBy(int x, int y); // moves the palette by a certain amount
		void SetPos(int x, int y);
		void SetSize(int x, int y);
};
#endif
