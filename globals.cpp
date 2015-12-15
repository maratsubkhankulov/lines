// This is the implementation of the Globals singleton class

// includes here
#include "globals.h"
#include "hgeColor.h"
#include "hgeFont.h"
#include "hgeanim.h"
#include "hge.h"
#include "hgeresource.h"

bool Globals::iFlag = false;
Globals* Globals::globals = 0;

Globals::Globals()
{
    hge = hgeCreate(HGE_VERSION);
    mouseX = 0;
    mouseY = 0;
    ballRockSpeed = 15;
    ballRockX = 0.4;
    ballRockY = 0.4;
    tileHighlight.SetHWColor(0xFFBBAAAA);
    tileLowHighlight.SetHWColor(0x77BBAAAA);
    tileNormal.SetHWColor(0xFFFFFFFF);
    colourStep.SetHWColor(0xf000000);
    meshColours[0] = 0xFFd56868;
    meshColours[1] = 0xFFd5c769;
    meshColours[2] = 0xFF67d3d4;
    meshColours[3] = 0xFF75d469;
    meshColours[4] = 0xFF6a68d2;
    meshColours[5] = 0xFFb769d4;
    meshColours[6] = 0xFFa29f9f;
    
    meshLostColours[0] = 0xFF000000;
    meshLostColours[2] = 0xFF010000;
    meshLostColours[1] = 0xFF000000;
    
    fontScore = 0x44000000;
    fontNormal = 0xFFFFFFFF;
        
    tileAnimDuration = 0.15f;
    
    recordScore = hge->Ini_GetInt("highscores","recordScore",10); //TESTING XXXXXXXXXXXXXXXX
    
    resManager = new hgeResourceManager("Graphics/resources.res");
    
    ballAnims[0] = resManager->GetAnimation("ballAnim1");
    ballAnims[1] = resManager->GetAnimation("ballAnim2");
    ballAnims[2] = resManager->GetAnimation("ballAnim3");
    ballAnims[3] = resManager->GetAnimation("ballAnim4");
    ballAnims[4] = resManager->GetAnimation("ballAnim5");
    ballAnims[5] = resManager->GetAnimation("ballAnim6");
    ballAnims[6] = resManager->GetAnimation("ballAnim7");
    
    ballPopAnims[0] = resManager->GetAnimation("ballPopAnim1");
    ballPopAnims[1] = resManager->GetAnimation("ballPopAnim2");
    ballPopAnims[2] = resManager->GetAnimation("ballPopAnim3");
    ballPopAnims[3] = resManager->GetAnimation("ballPopAnim4");
    ballPopAnims[4] = resManager->GetAnimation("ballPopAnim5");
    ballPopAnims[5] = resManager->GetAnimation("ballPopAnim6");
    ballPopAnims[6] = resManager->GetAnimation("ballPopAnim7");
    
    baseSpr = resManager->GetSprite("base");
    playerMiddleSpr = resManager->GetSprite("middleSection");
    playerTopSpr = resManager->GetSprite("playerTop");
    recordMiddleSpr = resManager->GetSprite("middleSection");
    recordTopSpr = resManager->GetSprite("recordTop");
    
    tileSpr = resManager->GetSprite("tile");
    cursorSpr = resManager->GetSprite("cursor");
    
    otherTex = resManager->GetTexture("other1");
    
    fnt = new hgeFont("Graphics/font1.fnt");
    tile_pop = hge->Effect_Load("Sounds/tile_pop.wav");
    ball_move = hge->Effect_Load("Sounds/ball_move.wav"); 
    game_over = hge->Effect_Load("Sounds/Gameover.wav"); 
    victory = hge->Effect_Load("Sounds/Victory.wav"); 
    ball_pop = hge->Effect_Load("Sounds/ball_pop.wav"); 
    
    notes[0] = hge->Effect_Load("Sounds/note1.wav");
    notes[1] = hge->Effect_Load("Sounds/note2.wav");
    notes[2] = hge->Effect_Load("Sounds/note3.wav");
    notes[3] = hge->Effect_Load("Sounds/note4.wav");
    notes[4] = hge->Effect_Load("Sounds/note5.wav");
    notes[5] = hge->Effect_Load("Sounds/note6.wav");
    notes[6] = hge->Effect_Load("Sounds/note7.wav"); 
}

Globals* Globals::getInstance()
{
    if (iFlag == false)
    {
        globals = new Globals();
        iFlag = true;
        return globals;
    } 
    else
    {
        return globals;
    }
}

Globals::~Globals()
{
    iFlag = false;
    // delete and release the resources
    hge->Effect_Free(tile_pop);
    hge->Effect_Free(ball_move);
    hge->Effect_Free(game_over);
    hge->Effect_Free(victory);
    hge->Effect_Free(ball_pop);
    hge->Texture_Free(otherTex);
    /*colourStep;
    hgeColor tileHighlight;
    hgeColor tileNormal;*/
    delete fnt;
    delete resManager;
    delete baseSpr;
    delete playerMiddleSpr;
    delete playerTopSpr;
    delete recordMiddleSpr;
    delete recordTopSpr;
    delete cursorSpr;
    delete tileSpr;
    delete[] ballAnims;
    delete[] ballPopAnims;
    delete[] notes;
    // Finally release the hge handle
    hge->Release();
}
