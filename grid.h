// The class for the game grid
// Renders itself

//To DO:
//Done:
//-grid->translatePosition() - renamed to getTileX(int position);

#ifndef GRID
#define GRID

#include "hgeSprite.h"
#include "hgeRect.h"
#include "globals.h"
#include "hgeColor.h"

class Grid
{
    public:
        Grid(int w, int h, hgeSprite *_gridTile);
        ~Grid();
        void render();
        void update(float dt);
        float getMarginX();
        float getMarginY();
        float getTileX(int w); // to implement
        float getTileY(int h); // to implement
        int getWidth(); // to implement
        int getHeight(); // to implement
        void highlight(int w, int h, bool flag);
        void specialHighlight(int w, int h, int offset);
        hgeRect* getRect(int w, int h);
        bool finishedSpecialHighlight();
        
        
    private:
        Globals* globals;
        hgeSprite* gridTile;
        hgeRect* gridRects[9][9];
        bool highlightGrid[9][9];
        bool specialHighlightGrid[9][9];
        float timeGrid[9][9];
        hgeColor colourGrid[9][9];
        int width;
        int height;
        float sizeX;
        float sizeY;
        float texSizeX;
        float texSizeY;
        float tileSizeX;
        float tileSizeY;
        float hscale;
        float vscale;
        float marginX;
        float marginY;
    
    // Possible addition of "gridTileSelected"
};
    
#endif
