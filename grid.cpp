// Implementation of Grid class' methods
//To dos:
//Resolved:
//- call createHGE() to find texSize, then release



#include "Grid.h"
#include "globals.h"

Grid::Grid(int w, int h, hgeSprite* _gridTile)
{
    globals = Globals::getInstance();
    width = w;
    height = h;
    gridTile = _gridTile;
    sizeX = 500;
    sizeY = 500;           
    tileSizeX = (float)sizeX/(float)w;
    tileSizeY = (float)sizeY/(float)h;
    texSizeX = _gridTile->GetWidth();
    texSizeY = _gridTile->GetHeight();
    hscale = tileSizeX/texSizeX;
    vscale = tileSizeY/texSizeY;
    marginX = (800-sizeX)/2;
    marginY = (600-sizeY)/2;
    for (int w = 0; w<width; w++)
    {
        for (int h = 0; h<height; h++)
        {
            float x1 = marginX+tileSizeX*w;
            float y1 = marginY+tileSizeY*h;
            float x2 = marginX+tileSizeX*w+tileSizeX;
            float y2 = marginY+tileSizeY*h+tileSizeY;
            gridRects[w][h] = new hgeRect(x1, y1, x2, y2);
            highlightGrid[w][h] = false;
            specialHighlightGrid[w][h] = false;
            timeGrid[w][h] = 0;
            colourGrid[w][h].SetHWColor(globals->tileNormal.GetHWColor());
        }
    }
}

Grid::~Grid()
{
    // Get rid of all the rects
    for (int w = 0; w<width; w++)
    {
        for (int h = 0; h<height; h++)
        {
            delete gridRects[w][h];
        }
    }
}

void Grid::render()
{
// scaling to size, render a Width x Height grid out of the grid tiles
    for (int w = 0; w<width; w++)
    {
        for (int h = 0; h<height; h++)
        {
            float x = gridRects[w][h]->x1;
            float y = gridRects[w][h]->y1;
            //highlight this tile
            gridTile->SetColor(colourGrid[w][h].GetHWColor());
            gridTile->RenderEx(x, y, 0, hscale, vscale);
        }
    }
}

void playSound(HEFFECT sound);

void Grid::update(float dt)
{
    // update the colour of tile if it is highlighted
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
           // if (highlightGrid[w][h] == true)
//            {
                if (timeGrid[w][h] > 0)
                {
                    timeGrid[w][h] -= dt;
                    colourGrid[w][h].SetHWColor(colourGrid[w][h].GetHWColor() - globals->colourStep.GetHWColor());
                    // if colour has reached black keep it black
                    if (colourGrid[w][h].GetHWColor() < globals->tileLowHighlight.GetHWColor())
                    {
                        colourGrid[w][h].SetHWColor(globals->tileLowHighlight.GetHWColor());
                    }
                } else
                {
                    if (specialHighlightGrid[w][h] == true)
                    {
                        specialHighlightGrid[w][h] = false;
                        playSound(globals->tile_pop);
                        colourGrid[w][h].SetHWColor(globals->tileNormal.GetHWColor());
                    }
                    if (highlightGrid[w][h] == true)
                    {
                        colourGrid[w][h].SetHWColor(globals->tileHighlight.GetHWColor());
                    }
                    if (highlightGrid[w][h] == false)
                    {
                        colourGrid[w][h].SetHWColor(globals->tileNormal.GetHWColor());
                                                
                    }
                }
            
        }
    }
}

float Grid::getMarginX()
{
    return marginX;
}

float Grid::getMarginY()
{
    return marginY;
}

float Grid::getTileX(int w)
{
    return marginX+w*tileSizeX; ///not the way it should be
}

float Grid::getTileY(int h)
{
    return marginY+h*tileSizeY; /// not the way it should be
}

int Grid::getWidth()
{
    return width;
}

int Grid::getHeight()
{
    return height;
}

hgeRect* Grid::getRect(int w, int h)
{
    return gridRects[w][h];
}

void Grid::highlight(int w, int h, bool flag)
{
    static int oldW = 0;
    static int oldH = 0;
    if (flag)
    {
        highlightGrid[oldW][oldH] = false;
        timeGrid[oldW][oldH] = 0;
        colourGrid[oldW][oldH] = globals->tileNormal;
        highlightGrid[w][h] = true;
        timeGrid[w][h] = globals->tileAnimDuration;
        oldW = w;
        oldH = h;
    } else
    {
        highlightGrid[w][h] = false;
        timeGrid[w][h] = 0;
        colourGrid[oldW][oldH] = globals->tileNormal;
    }
}

void Grid::specialHighlight(int w, int h, int offset)
{
     specialHighlightGrid[w][h] = true;
     timeGrid[w][h] = globals->tileAnimDuration+((float)offset)/50;
}

bool Grid::finishedSpecialHighlight()
{
    bool fin = true;
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            if (specialHighlightGrid[w][h] == true) fin = false;
        }
    }
    
    return fin;
}
