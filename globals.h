// This is the Globals class which hold information
// The idea is that this can be accessed from anywhere
// Within the game

// This class follows the Singleton pattern

#ifndef GLOBALS_H
#define GLOBALS_H

#include "hge.h"
#include "hgeColor.h"
#include "hgeFont.h"
#include "hgeresource.h"
#include "hgesprite.h"
#include "hgeanim.h"

class Globals
{
    public:
        static Globals* getInstance();
        ~Globals();
        float mouseX;
        float mouseY;
        float ballRockSpeed;
        float ballRockX;
        float ballRockY;
        float tileAnimDuration; // milliseconds
        hgeColor colourStep;
        hgeColor tileHighlight;
        hgeColor tileLowHighlight;
        hgeColor tileNormal;
        hgeFont* fnt;
        HEFFECT tile_pop;
        HEFFECT ball_move;
        HEFFECT game_over;
        HEFFECT victory;
        HEFFECT ball_pop;
        HEFFECT notes[7];
        HGE* hge;
        hgeAnimation* ballAnims[7];
        hgeAnimation* ballPopAnims[7];
        //hgeSprite* spr; // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        hgeSprite *baseSpr;
        hgeSprite *playerMiddleSpr;
        hgeSprite *playerTopSpr;
        hgeSprite *recordMiddleSpr;
        hgeSprite *recordTopSpr;
        int recordScore;
        DWORD meshColours[7];
        DWORD meshLostColours[3];
        DWORD fontScore;
        DWORD fontNormal;
        hgeSprite *tileSpr;
        hgeSprite *cursorSpr;
        HTEXTURE otherTex;
        
    private:
        hgeResourceManager* resManager;
        Globals();
        static Globals* globals;
        static bool iFlag;
};

#endif
