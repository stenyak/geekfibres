#include "palette.h"

vector<ThumbData *> Palette::paletteThumbs;

typedef struct
{
	SDL_Rect rect;
	string name;

}PaletteTileData;


void Palette::addToVector(pair<string, TileData *> mypair)
{
    TileData * tile = mypair.second;
    for (int i=0; i<tile->tileImages->GetSize();i++)
    {
        ThumbData * thumb = new ThumbData;
        thumb->tileName = tile->tileName;
        thumb->integrityLevel=i+1;
        paletteThumbs.push_back(thumb);
    }
}


Palette::Palette(LevelData * levelData)
{
	this->levelData = levelData;

    buttons.push_back(IMG_Load((ConfigManager::gamedataPath + string("gui/Back.png")).c_str()));
    buttons.push_back(IMG_Load((ConfigManager::gamedataPath + string("gui/Forward.png")).c_str()));
    buttons.push_back(IMG_Load((ConfigManager::gamedataPath + string("gui/BackDisabled.png")).c_str()));                      
    buttons.push_back(IMG_Load((ConfigManager::gamedataPath + string("gui/ForwardDisabled.png")).c_str()));
    
    for(int i=0;i<2;i++)
    {
        buttonRects[i].h = buttons.at(i)->h;
        buttonRects[i].w = buttons.at(i)->w;
    }
    
	scrollAmount =0;
	SetSize(ConfigManager::GetIntProperty("paletteWidth"), ConfigManager::GetIntProperty("paletteHeight"));
	SetPos(ConfigManager::GetIntProperty("paletteX"),ConfigManager::GetIntProperty("paletteY"));
    for_each(TileBuilder::tileTypes.begin(),TileBuilder::tileTypes.end(), &(addToVector));
    currentTileType =NULL;
      
}

ThumbData * Palette::GetCurrentTileType()
{
	return currentTileType;
}

void Palette::Draw(SDL_Surface * screen)
{
	// calculate the number of tiles that should be displayed based on the scrollAmount variable, 
	// the size of the palette and tiles size

    int vTilesNumber = (int) floor(innerPaletteRect.h/(float)DIMENSION_TILE);
    int hTilesNumber = (int) floor(innerPaletteRect.w/(float)DIMENSION_TILE);
    
	// store the SDL_Rects for the displayed palette tiles in a vector, so as to be able to retrieve
	// them and compare collisions in the ProcessEvents function
    int totalThumbs = paletteThumbs.size();
    int totalRows = (int) ceil(totalThumbs/(float)hTilesNumber);
    if(scrollAmount>=totalRows)
        scrollAmount=totalRows-1;
    int firstThumb = scrollAmount * hTilesNumber;
    tilesOnDisplay.clear();
    for(int i=0;i<hTilesNumber*vTilesNumber && i+firstThumb<totalThumbs;i++)
    {
        DisplayedTileData * dtd = new DisplayedTileData;
        dtd->thumb = paletteThumbs.at(i+firstThumb);
        dtd->rect.h = DIMENSION_TILE;
        dtd->rect.w = DIMENSION_TILE;
        dtd->rect.x = i%hTilesNumber*DIMENSION_TILE + innerPaletteRect.x + 2;
        dtd->rect.y = i/hTilesNumber*DIMENSION_TILE + innerPaletteRect.y + 2 ; 
        tilesOnDisplay.push_back(dtd);
    }
    
	// draw the outer palette rectangle 
    int r = ConfigManager::GetIntProperty("paletteBorderColorRed");
    int g = ConfigManager::GetIntProperty("paletteBorderColorGreen");
    int b = ConfigManager::GetIntProperty("paletteBorderColorBlue");

	SDL_FillRect(screen,&borderPaletteRect,SDL_MapRGB(screen->format,r,g,b));

    r = ConfigManager::GetIntProperty("paletteInnerColorRed");
    g = ConfigManager::GetIntProperty("paletteInnerColorGreen");
    b = ConfigManager::GetIntProperty("paletteInnerColorBlue");
                  
	// draw the inner palette rectangle
	SDL_FillRect(screen,&innerPaletteRect,SDL_MapRGB(screen->format,r,g,b));

	// draw the tiles wich need to be drawn based on the current scrollAmount
    vector<DisplayedTileData *>::iterator it;
    for(it = tilesOnDisplay.begin();it!=tilesOnDisplay.end();it++)
    {
        SDL_BlitSurface (
                         TileBuilder::GetTileDataByName((*it)->thumb->tileName)->tileImages->GetImagen((*it)->thumb->integrityLevel),
                         NULL, screen, &((*it)->rect));
    }
    for(int i=0;i<2;i++)
    {
        SDL_BlitSurface(buttons[i],NULL,screen,&buttonRects[i]);
    }
}
		
bool PointRectCollision(SDL_Rect * rect, int pointX, int  pointY)
{
    if (pointX< rect->x)
        return false;
    if(pointY<rect->y)
        return false;
    if(pointX>rect->x+rect->w)
        return false;
    if(pointY>rect->y+rect->h)
        return false;
    return true;
}

ThumbData * Palette::CollidedWithAnyThumbData(int x, int y)
{
    vector<DisplayedTileData *>::iterator it;
    for(it = tilesOnDisplay.begin();it!=tilesOnDisplay.end();it++)
        if(PointRectCollision(&(*it)->rect,x,y))
            return (*it)->thumb;
    
    return NULL;
}

void Palette::ProcessEvents()
{

	// add a special "markup" event to the queue, so that we can recognize it when we get to it
    SDL_Event * ev = new SDL_Event;
    ev->type = 255;
    SDL_PushEvent(ev);
    
    // we travel the event queue until we find the markup event 
    SDL_PollEvent(ev);
	while(ev->type!=255)
    {
        // check if it's a mouse event (we are only interested in those)
        if(ev->type != SDL_MOUSEBUTTONDOWN && ev->type!=SDL_MOUSEMOTION)
        {
            SDL_PushEvent(ev);
            SDL_PollEvent(ev);
            continue;
        }
    
        static bool dragging =false; //indicates if the palette is being dragged atm
        
        // check if the coordinates are inside the palette
        if(PointRectCollision(&borderPaletteRect,ev->button.x, ev->button.y))
        {
            // if it was a mouse motion event and a button was pressed, it will move the palette
            if(ev->type == SDL_MOUSEMOTION && ev->motion.state!=0) 
            {
                dragging=true;
                this->MoveBy(ev->motion.xrel,ev->motion.yrel);
            }
            //check if the coordinates are in the inner section of the palette
            
            else if( ev->type == SDL_MOUSEBUTTONDOWN && PointRectCollision(&innerPaletteRect, ev->button.x, ev->button.y))
            {
                ThumbData * thumb = CollidedWithAnyThumbData(ev->button.x,ev->button.y);
                if(thumb!=NULL)
                {
                    currentTileType = thumb;
                }
            }
            else if(ev->type == SDL_MOUSEBUTTONDOWN)
            { 
                if(PointRectCollision(&buttonRects[0],ev->button.x,ev->button.y))
                { // pressed the "back" button
                    scrollAmount--; 
                    if(scrollAmount<0)
                        scrollAmount=0;
                }
                else if(PointRectCollision(&buttonRects[1],ev->button.x,ev->button.y))
                { // pressed the "forward" button
                    scrollAmount++; 
                }
            }
                    
        }
        else
        {
            if(ev->type == SDL_MOUSEMOTION)
            {
                if(dragging && ev->motion.state!=0)
                {
                    this->MoveBy(ev->motion.xrel,ev->motion.yrel);
                }
                else
                {
                    SDL_PushEvent(ev);
                    if(ev->motion.state==0)
                    {
                        dragging=false;
                    }
                }
            }
            else
            {
                SDL_PushEvent(ev);
            }
        }
        SDL_PollEvent(ev);
    }
}

void Palette::MoveBy(int x, int y)
{
	SetPos( x+borderPaletteRect.x , y+borderPaletteRect.y); 
}

void Palette::SetPos(int x, int y)
{
	// check if the new position takes the palette out of the screen.
	// if it does, calculate the position that keeps the palette inside of the screen
	// Check north
	if (borderPaletteRect.h + y > ConfigManager::V_RES)
		y = ConfigManager::V_RES - borderPaletteRect.h;
	// check south
	if(y<0)
		y=0;
	// check west
	if(x<0)
		x=0;
	// check east
	if(borderPaletteRect.w+x>ConfigManager::H_RES)
		x = ConfigManager::H_RES-borderPaletteRect.w;

	// set the palette position to the calculated position
	// (all the palette rects must be updated)
	borderPaletteRect.x = x;
	borderPaletteRect.y = y;
	
	innerPaletteRect.x = x+horizontalWindowBorder;
	innerPaletteRect.y = y+upperWindowBorder;
    
    buttonRects[0].x = innerPaletteRect.x;
    buttonRects[0].y = y;
    
    buttonRects[1].x = innerPaletteRect.x+innerPaletteRect.w - buttonRects[1].w;
    buttonRects[1].y = y;
    
    
}

void Palette::SetSize(int width, int height)
{
	if(width ==0 || height==0)
		return;
	width = abs(width);
	height = abs(height);

	borderPaletteRect.w = width;
	borderPaletteRect.h = height;

	innerPaletteRect.w = width - 2*horizontalWindowBorder;
	innerPaletteRect.h = height - (lowerWindowBorder + upperWindowBorder);

	SetPos(borderPaletteRect.x, borderPaletteRect.y);
}	
