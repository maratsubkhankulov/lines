//This is a Lines Games clone with a few graphical twists
//v1.0
//By Marat Subkhankulov

// MIGHTS, COULDS AND SHOULDS:
//-Fix unfilled space bug
//-Implement functions to do with moving ball
//-Should store all global data in the globals singleton
//-Might use start()(DONE), initialize(), gameOver() instead of destroying and creating the game anew
//-The hscales of all the Balls should be calculated once
//-*hge must be used within a global class and make all the data available from there
//-Might add turnOffVolume()
//-bSound should signify no sound in the game. Could use a global sound level too.
//// Probably shouldn't worry:
//-"Ready" text doesn't always get drawn. I wonder why.
//-Game should not crash after many restarts, could be due to the deletion of the game object
//// Resolved:
//-Debug play, reload, click ball error
//-The balls should recieve clicks.
//-The menu should be rearranged and the sound on ball click fixed
//-After you start to play a game and go back to the menu, it is useful to give options
//to "restart" or "resume" game. Would this require a new gui? Could hide those buttons
//or simply alter the current gui to rename "play" and add "restart" items.
//-name bSound could change to bMusic
//-Could use @strings for later translation, not much text in the game though.
//-Should add a nicer background image. Maybe a family photo, or better yet a slide show.
//-The texture sizes need to be power of 2 for everything to work.
//Thus throw textures into one image and adjust from there.
//-Might write wrapper functions pause(), resume() which take into consideration bSound
//-ideally the channel with the intro music playing will be paused and resumed when entering the menu
//-when loading graphics "\" or "/"?
//-frameMenu might not work as it doesn't return a bool and I don't control that anyway

#include "hge.h"
#include "globals.h"
#include "hgeSprite.h"
#include "hgeanim.h"
#include "hgefont.h"
#include "hgegui.h"
#include "menuitem.h"
#include "game.h"
#include "exception.h"
#include <excpt.h>
#include "hgedistort.h"

#include <math.h>

// The global handles
HGE *hge = 0;
Globals *globals = 0;

//////The states of the game:
const int MENU = 0;
const int READY = 1;
const int BALL_SELECTED = 2;
const int BALL_MOVING = 3;
const int GENERATING_BALLS = 4;
const int POPPING = 5;
const int LOST = 6;
const int VICTORY = 7;

//////The modes on the game:
const int MODE_1 = 1; //Show nothing
const int MODE_2 = 2; //Show colour
const int MODE_3 = 3; //Show colour and next position

//Menu cases
const int MENU_RESUME = 1;
const int MENU_START = 2;
const int MENU_MODE = 3;
const int MENU_MUSIC = 4; // replaced for MENU_SOUND
const int MENU_SOUND = 5;
const int MENU_EXIT = 6;

//Strings to be translated
char* butPlay = "Play";
char* butMode = "Mode";
char* butSound = "Sound:";
char* butMusic = "Music:";
char* butExit = "Exit";
char* butRestart = "Restart";
char* butResume = "Resume";

char* msgBirthday = "Happy Birthday\npapa!";
char* msg = "game output";
char* msgReady = "Ready";
char* msgBallSelected = "Ball Selected";
char* msgBallMoving = "Ball Moving";
char* msgGeneratingBalls = "Generating Balls";
char* msgPoppingBalls = "Popping balls";
char* msgLost = "Game over\nYour score is: %i";
char* msgVictory = "Congratulations!\nYou set a new record: %i";
char* msgFrameError = "Ooh-oh something went wrong in the main FrameFunc";
char* txtTitle = "Lines Game";
char* txtYes = "Yes";
char* txtNo = "No";
char* txtOn = "On";
char* txtOff = "Off";

//Game controls
bool bMusic = true;
bool bSound = true;
bool bGameover = false;
bool bExitOnce = true;
int soundVolume = 50;

//Global constants
const int menuX = 300;
const int menuY = 200;
const int nRows = 8;
const int nCols = 6;
const float playerBaseX = 10;
const float playerBaseY = 500;
const float recordBaseX = 670;
const float recordBaseY = 500;
const int statueMaxHeight = 200;
const float predBallX = 200;
const float predBallY = 550;
const float statueGrowthRatio = 7;

//Global Handles
hgeGUI			    *gui;
hgeFont				*fnt;
hgeSprite           *cursorSpr;
hgeSprite           *tileSpr;
hgeSprite           *baseSpr;
hgeSprite           *playerMiddleSpr;
hgeSprite           *playerTopSpr;
hgeSprite           *recordMiddleSpr;
hgeSprite           *recordTopSpr;
hgeAnimation        *ballAnims[7];
hgeAnimation        *ballPopAnims[7];
hgeDistortionMesh    *mesh;

Game                *game;

//Resource Handles
HCHANNEL            introChannel;
HCHANNEL            gameChannel;
//HCHANNEL            ballClickChannel; // not used
HEFFECT             intro;
HEFFECT				snd;
HEFFECT             ball_click;
//HEFFECT             ball_move;
HEFFECT             tile_pop;
HEFFECT             game_over;
HEFFECT             victory;
//HTEXTURE            ballsTex;
//HTEXTURE			cursorTex;
//HTEXTURE            tileTex;
hgeQuad				quad;

//Global variables
int state = MENU;
int oldState = READY;
int mode = MODE_1;
float mouseX;
float mouseY;
float playerMiddleHeight = 2;
float recordMiddleHeight = 2;

HCHANNEL playSound(HEFFECT sound)
{
     if (bSound)
     {
         HCHANNEL c = hge->Effect_PlayEx(sound);
         return c;
     }
     return 0;
}

void pauseMusic(HCHANNEL c)
{
    hge->Channel_Pause(c);
}

void resumeMusic(HCHANNEL c)
{
    if (bMusic&&bSound){
        hge->Channel_Resume(c);
    }
}

bool musicOver(HCHANNEL c)
{
    if (bMusic&&bSound&&!hge->Channel_IsPlaying(c))
    {
        return true;
    }
    return false;
}
// Updates the length of the middle section of the statues in accordance with the score
void updateStatues()
{
    playerMiddleHeight = (game->getScore()/statueGrowthRatio);//%statueMaxHeight; // TO BE FIXED XXXXXXXXXXXXXXXXXXXXXXXXXX
    recordMiddleHeight = (globals->recordScore/statueGrowthRatio);//%statueMaxHeight;
}

void cycleMode()
{
     if (mode == MODE_1){ mode = MODE_2; return;}
     if (mode == MODE_2){ mode = MODE_3; return;}
     if (mode == MODE_3){ mode = MODE_1; return;}
}

bool frameMenu(float dt)
{
	int id;
	static int lastid=0;
	
	//globals->ballAnims[0]->Update(dt); // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXX
	
	// We update the GUI and take an action if
	// one of the menu items was selected
	id=gui->Update(dt);
	if(id == -1)
	{
		switch(lastid)
		{
			case MENU_RESUME:
                // Resume current game
                state = oldState;
                pauseMusic(introChannel);
				break;
			case MENU_START:
                // Play new/restart game
                state = READY;
                bGameover = false;
                bExitOnce = true;
                // delete old game
                delete game;
                // might want to create a separate function for this
                game = new Game(mode,&state);
                game->setGrid(9,9,tileSpr);
                game->setBalls(ballAnims, ballPopAnims, 7);
                game->start();
                
                updateStatues();
                
                //wipe clear the mesh
                for(int i=1;i<nCols;i++)
					for(int j=1;j<nRows;j++)
					{
						mesh->SetColor(j,i,0x00000000);
					}
				break;
			case MENU_MODE:
                gui->SetFocus(MENU_MODE);
				gui->Enter();
				cycleMode();
				break;
			case MENU_SOUND:
                gui->SetFocus(MENU_SOUND);
				gui->Enter();
                if (bSound){
                    bSound = false;
                    pauseMusic(introChannel);
                    break;
                } else {
                    bSound = true;
                    resumeMusic(introChannel);
                    break;
                }
                break;
            case MENU_MUSIC:
                gui->SetFocus(MENU_SOUND);
				gui->Enter();
                if (bMusic){
                    bMusic = false;
                    pauseMusic(introChannel);
                    break;
                } else {
                    bMusic = true;
                    resumeMusic(introChannel);
                    break;
                }
                break;
            case MENU_EXIT: return true;
                
		}
	}
	else if(id) { lastid=id; gui->Leave();}
	
	return false;
}

void renderMenu()
{
    //hge->Gfx_RenderQuad(&quad);
    hge->Gfx_Clear(0);
    	static float bgX = 400;
    static float bgY = 700;
    static float bgRot = 1;
    static float bgScale = 8;
    static float vX = 0;
    static float vY = 0;
    static float rockY = 0;
    static float positionX = 650;
    static float positionY = -100;
    static float rockRate = 0.01;
    static float moveRate = 0.01;
    static float amplitudeX = 0.01;
    static float amplitudeY = 0.3;
    static int randomI = hge->Random_Int(0,6);
    static bool reached = false;
    
    rockY+=rockRate;
    reached = true;
    bgX += sin(rockY)*amplitudeX;
    bgY += sin(rockY)*amplitudeY;
    bgY -= moveRate*(bgY-positionY);
    bgX -= moveRate*(bgX-positionX);
                                         
   	globals->ballAnims[4]->RenderEx(bgX,bgY,bgRot,bgScale,bgScale);
   	fnt->SetScale(2);
   	fnt->SetColor(0xFFFFFFFF);
   	fnt->SetBlendMode(BLEND_ALPHAADD);
    fnt->printf(bgX-65,bgY+290, HGETEXT_CENTER, msgBirthday);
    fnt->SetBlendMode(BLEND_DEFAULT);
    //fnt->SetColor(0xFFFFFFFF);
    fnt->SetScale(1);
    
    fnt->printf(menuX+20,menuY+80, HGETEXT_LEFT, "%i", mode);
    fnt->printf(menuX+20,menuY+120, HGETEXT_LEFT, "%s", bSound?txtOn:txtOff);
    fnt->printf(menuX+20,menuY+160, HGETEXT_LEFT, "%s", bMusic?txtOn:txtOff);
	gui->Render();
	
}

void frameReady(float dt)
{
     game->update(dt);
}

void renderBase()
{
     hge->Gfx_Clear(0);
     game->render();
     fnt->SetColor(0xFFFFFFFF);
     //fnt->printf(460,240, HGETEXT_LEFT, msg);
     // Render player piedestal
         // The base
         baseSpr->Render(playerBaseX, playerBaseY);
         
         // Middle section
         playerMiddleSpr->RenderStretch(playerBaseX, playerBaseY-playerMiddleHeight-25,
                                         playerBaseX+playerMiddleSpr->GetWidth(), playerBaseY+25);
         
         // Top
         playerTopSpr->Render(playerBaseX,
                              playerBaseY-playerMiddleHeight-playerTopSpr->GetHeight());    
     
          // The base
         baseSpr->Render(playerBaseX, playerBaseY);
         
         // Render record piedestal
         
         
         // Middle section
         recordMiddleSpr->RenderStretch(recordBaseX, recordBaseY-recordMiddleHeight-25,
                                         recordBaseX+recordMiddleSpr->GetWidth(), recordBaseY+25);
         
         // The base
         baseSpr->Render(recordBaseX, recordBaseY);
         
         // Top
         recordTopSpr->Render(recordBaseX,
                              recordBaseY-recordMiddleHeight-recordTopSpr->GetHeight());
     
     // Render game predictions
     static float scale = 0.5;
     static float offset = game->ballPred[0]->GetWidth()*0.5;
     switch(mode)
     {
         case(MODE_3):
             // Render positions
             
             for (int i = 0; i < game->nPredicted; i++)
                 if (game->ballPred[i] != NULL) game->ballPred[i]->RenderEx(game->getPredX(i), game->getPredY(i), 0, scale, scale);
             // Render colours predicted a further move
             
             game->ballPred2[0]->RenderEx(predBallX, predBallY, 0, scale, scale);
             game->ballPred2[1]->RenderEx(predBallX+offset, predBallY, 0, scale, scale);
             game->ballPred2[2]->RenderEx(predBallX+offset*2, predBallY, 0, scale, scale);
             break;
         case(MODE_2):
             // Render colours
             
             game->ballPred[0]->RenderEx(predBallX, predBallY, 0, scale, scale);
             game->ballPred[1]->RenderEx(predBallX+offset, predBallY, 0, scale, scale);
             game->ballPred[2]->RenderEx(predBallX+offset*2, predBallY, 0, scale, scale);
     }                       
     
     fnt->SetColor(globals->fontScore);                         
     fnt->printf(playerBaseX+baseSpr->GetWidth()/2 ,playerBaseY-10, HGETEXT_CENTER, "%i", game->getScore());
     fnt->printf(recordBaseX+baseSpr->GetWidth()/2 ,recordBaseY-10, HGETEXT_CENTER, "%i", globals->recordScore);
     fnt->SetColor(globals->fontNormal);
}

void renderReady()
{
     renderBase();
     //fnt->printf(460,220, HGETEXT_LEFT, msgReady);
}

void renderBallSelected()
{
     renderBase();
     //fnt->printf(460,220, HGETEXT_LEFT, msgBallSelected);
}

void renderBallMoving()
{
     renderBase();    
     //fnt->printf(460,220, HGETEXT_LEFT, msgBallMoving);
}

void renderGeneratingBalls()
{
     renderBase();
     //fnt->printf(460,220, HGETEXT_LEFT, msgGeneratingBalls);
}

void renderPopping()
{
     renderBase();
     //fnt->printf(460,220, HGETEXT_LEFT, msgPoppingBalls);
}

void renderLost()
{
     renderBase();
     //draw the screen
     mesh->Render(0,0);
     fnt->SetScale(2.0f);
     fnt->printf(400,220, HGETEXT_CENTER, msgLost, game->getScore());
     fnt->SetScale(1.0f);    
}

void renderVictory()
{
     renderBase();
     //draw the screen
     mesh->Render(0,0);
     fnt->SetScale(2.0f);
     fnt->printf(400,200, HGETEXT_CENTER, msgVictory, globals->recordScore);
     fnt->SetScale(1.0f);     
}

//In this function we calculate what needs to be done depending on the state
bool FrameFunc()
{
    float dt=hge->Timer_GetDelta();
    static float t = 0;
	static int wa = 0;
    static int ha = 0;
    t += 0.3*dt;
	switch (state){
        case MENU: 
             if (hge->Input_KeyDown(HGEK_ESCAPE)) return true;
             if(frameMenu(dt)) return true;
             if (game != NULL) if (game->checkNewRecord()) 
             {
                 globals->recordScore = game->getScore();
                 hge->Ini_SetInt("highscores","recordScore",globals->recordScore);
             }
             break;
        case READY:
             // Check if a ball has been clicked -> state = BALL_SELECTED
             if (game->onClickBall() == true) 
             {
                 msg = "ball clicked";
                 state = BALL_SELECTED;
                 //playSound(ball_click); // Now done by the ball itself
             }             
             hge->Channel_Pause(introChannel);
             frameReady(dt);
             //Do this once and straight away
             // Alter menu: replace play with restart, add resume.
             static bool firstTime = true;
             if (firstTime)
             {
                gui->DelCtrl(MENU_START);
                gui->DelCtrl(MENU_MODE);
                gui->DelCtrl(MENU_SOUND);
                gui->DelCtrl(MENU_MUSIC);
                gui->DelCtrl(MENU_EXIT);
                gui->AddCtrl(new hgeGUIMenuItem(MENU_RESUME,fnt,snd,menuX,menuY,0.1f,butResume));
                gui->AddCtrl(new hgeGUIMenuItem(MENU_START,fnt,snd,menuX,menuY+40,0.2f,butRestart));
            	gui->AddCtrl(new hgeGUIMenuItem(MENU_MODE,fnt,snd,menuX,menuY+80,0.3f,butMode));
            	gui->AddCtrl(new hgeGUIMenuItem(MENU_SOUND,fnt,snd,menuX,menuY+120,0.4f,butSound));
            	gui->AddCtrl(new hgeGUIMenuItem(MENU_MUSIC,fnt,snd,menuX,menuY+160,0.5f,butMusic));
            	gui->AddCtrl(new hgeGUIMenuItem(MENU_EXIT,fnt,snd,menuX,menuY+200,0.6f,butExit));
            	firstTime = false;
             }
             //On escape key...
	         if (hge->Input_KeyDown(HGEK_ESCAPE))
             {
                state = MENU;
                // remember which state to come back to
                oldState = READY;
                gui->SetFocus(1);
                gui->Enter();
                resumeMusic(introChannel); 
             }
             break;
        case BALL_SELECTED:
             {
                 // this can go 3 ways
                 // either the ball is moved to a new tile -> state= BALL_MOVING
                 // or the ball a tile is clicked but the ball can't get there -> state= READY
                 // or a different ball is selected -> BALL_SELECTED
                 game->update(dt);
                 // on click tile - try to move the ball there
                 if (game->onClickBall() == true) 
                 {
                     msg = "ball clicked";
                     state = BALL_SELECTED;
                     //playSound(ball_click); // Now done by the ball itself
                 } else if (game->onClickTile(&wa, &ha))
                 {
                     msg = "tile clicked";
                     if(game->findPath())
                     {
                         game->highlightPath();
                         //game->moveBall();
                         state = BALL_MOVING; // SHOULD BE BALL_MOVING WWWWWWWWW!!!!!!!!!!!
                         //playSound(ball_move); // Now the sound is played inside the grid class
                     }
                 }
                 
                 // the menu and music code from READY goes here aswell
                 //On escape key...
    	         if (hge->Input_KeyDown(HGEK_ESCAPE))
                 {
                    state = MENU;
                    // remember which state to come back to
                    oldState = BALL_SELECTED;
                    gui->SetFocus(1);
                    gui->Enter();
                    resumeMusic(introChannel); 
                 }
                 break;        
             }
        case BALL_MOVING:
             {
                 game->update(dt);
                 //Check to see if any lines of 5 have been made
                 //If so pop those lines, calculate scores and start next turn
                 if (game->finishedSpecialHighlight())
                 {
                     game->moveBall();
                     
                     if (game->findCompleteLines())
                     {
                         msg = "Popping!";
                         game->pop(); // encapsulates score calculation
                         updateStatues();
                         oldState = BALL_MOVING;
                         state = POPPING;
                     } else
                     {
                         state = GENERATING_BALLS; // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx
                     }
                 }
                 //On escape key...
    	         if (hge->Input_KeyDown(HGEK_ESCAPE))
                 {
                    state = MENU;
                    // remember which state to come back to
                    oldState = BALL_MOVING;
                    gui->SetFocus(1);
                    gui->Enter();
                    resumeMusic(introChannel); 
                 }
                 break; 
             }
        case GENERATING_BALLS:
             {
                 game->update(dt);
                 //Try to generate balls
                 //If sucessful check again in there are any complete lines
                 //Else the game is over
                 //try
                 //{
                     // TESTING THE TRY CATCH CLAUSE
                     if (!game->spawnBalls() && !game->gridIsFull())
                     {
                         if (game->findCompleteLines())
                         {
                             game->pop(); // Contains in it score calculation and update
                             game->predictPositions(); // Space has been uncovered make a new prediction
                             // MIGHT BE INCORRECT BEHAVIOUR XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                             updateStatues();
                             oldState = GENERATING_BALLS;
                             state = POPPING;
                         } else
                         {
                             state = READY;
                             game->newTurn();
                         }
                     } else
                     {
                         // Game over
                         // Display scores
                         // If new record then it's a victory
                         // Else loss
                         if (game->checkNewRecord())
                         {
                             state = VICTORY;
                             if (bMusic) gameChannel = playSound(victory);
                         } else
                         {
                             state = LOST;
                             if (bMusic) gameChannel = playSound(game_over);
                         }
                     }
                 //}
                 //catch(CException e)
                 //{
                   //  e.Report();
                 //}
                 
                 if (hge->Input_KeyDown(HGEK_ESCAPE))
                 {
                    state = MENU;                                                 
                    // remember which state to come back to
                    oldState = GENERATING_BALLS;
                    gui->SetFocus(1);
                    gui->Enter();
                    resumeMusic(introChannel); 
                 }
                 break;
             }
        case POPPING:{
                 game->update(dt);
                 if (game->popped())
                 {
                     state = READY;
                 }
                 if (hge->Input_KeyDown(HGEK_ESCAPE))
                 {
                    state = MENU;                                                 
                    // remember which state to come back to
                    oldState = POPPING;
                    gui->SetFocus(1);
                    gui->Enter();
                    resumeMusic(introChannel); 
                 }
                 break;
             }
        case LOST:
             {
                 // read out score
                 // play loser music
                 //game->gameOver(dt);
                 for(int i=1;i<nCols-1;i++)
					for(int j=1;j<nRows-1;j++)
					{
                        mesh->SetDisplacement(j,i,cosf(t*10+(i+j)/2)*5,sinf(t*10+(i+j)/2)*5,HGEDISP_NODE);
						if (hge->Random_Int(0,100) < 50) mesh->SetColor(j,i,globals->meshLostColours[hge->Random_Int(0,2)]);
					}
                 if (hge->Input_KeyDown(HGEK_ESCAPE)||(musicOver(gameChannel)&&bExitOnce))
                 {
                    bExitOnce = false;
                    state = MENU;                                                 
                    // remember which state to come back to
                    oldState = LOST;
                    gui->SetFocus(1);
                    gui->Enter();
                    pauseMusic(gameChannel);
                    resumeMusic(introChannel); 
                 }
                 break;
             }
        case VICTORY:{
                 // player wins if a new record is set
                 // read out score
                 // save score
                 // play music
                 //playSound(victory);
                 globals->recordScore = game->getScore();
                 hge->Ini_SetInt("highscores","recordScore",globals->recordScore);
                 // play winner music
                 game->update(dt);
                 // update the background mesh
                 for(int i=1;i<nCols-1;i++)
					for(int j=1;j<nRows-1;j++)
					{
                        mesh->SetDisplacement(j,i,cosf(t*10+(i+j)/2)*5,sinf(t*10+(i+j)/2)*5,HGEDISP_NODE);
						if (hge->Random_Int(0,100) < 1) mesh->SetColor(j,i,globals->meshColours[hge->Random_Int(0,5)]);
					}
                 if (hge->Input_KeyDown(HGEK_ESCAPE)||(musicOver(gameChannel)&&bExitOnce))
                 {
                    bExitOnce = false;
                    state = MENU;                                                 
                    // remember which state to come back to
                    oldState = VICTORY;
                    gui->SetFocus(1);
                    gui->Enter();
                    pauseMusic(gameChannel);
                    resumeMusic(introChannel); 
                 }
                 break;
             }
        default:{(NULL, msgFrameError, "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);} //show error message
    }
    //Split on state and mode
	return false;
}

//This is the render function in which everything is drawn
bool RenderFunc()
{
    // Render graphics
	hge->Gfx_BeginScene();
    //Split on state and mode
   	switch (state){
        case MENU: 
             renderMenu(); // draw the menu
             break;      
        case READY: 
             renderReady(); // draw the playing field
             break;
        case BALL_SELECTED:{
             renderBallSelected();
             break;
             }
        case BALL_MOVING:{
             renderBallMoving();
             break;
             }
        case GENERATING_BALLS:{
             renderGeneratingBalls();
             break;
             }
        case POPPING:{
             renderPopping();
             break;
             }
        case LOST:{
             renderLost();
             break;
             }
        case VICTORY:{
             renderVictory();
             break;
             }
        default:{} //show error message
    }
    
    hge->Input_GetMousePos(&globals->mouseX, &globals->mouseY);
    cursorSpr->Render(globals->mouseX, globals->mouseY);
	hge->Gfx_EndScene();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	// Set our frame function
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	// Set the window title
	hge->System_SetState(HGE_TITLE, "Lines Game");
	// Run in windowed mode
	// Default window size is 800x600
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_FPS, HGEFPS_VSYNC);
	hge->System_SetState(HGE_INIFILE, "ini.txt");
	// Use BASS for sound
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
        // Create the Globals content holder
        globals = Globals::getInstance();
        
		// Load sound and textures
		quad.tex=globals->otherTex;
        //cursorTex=hge->Texture_Load("Graphics/cursor.png");
        //hge->Texture_Load("Graphics/cursor.png");
		snd=hge->Effect_Load("Sounds/menu_sound.wav");
		//ball_click=hge->Effect_Load("Sounds/ball_click.wav");
		intro=hge->Effect_Load("Sounds/intro.wav");
		game_over = globals->game_over;
		victory = globals->victory;
		tile_pop=globals->tile_pop;
		//ball_move=globals->ball_move;
		fnt=globals->fnt;
		//tileTex = hge->Texture_Load("Graphics/gridTile.png"); // REDUNDANT
		
		// Load the ball textures
		//ballsTex = hge->Texture_Load("Graphics/ballsTex.png"); // REDUNDANT
		
		// Set up the quad we will use for background animation
		quad.blend= BLEND_ALPHABLEND | BLEND_COLORMUL;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFFFFF;
		}

        quad.v[0].tx=0;        quad.v[0].ty=0;
    	quad.v[1].tx=(800.0f/1024.0f);        quad.v[1].ty=0;
    	quad.v[2].tx=(800.0f/1024.0f);        quad.v[2].ty=600.0f/1024.0f;
    	quad.v[3].tx=0;        quad.v[3].ty=600.0f/1024.0f;

		quad.v[0].x=-1; quad.v[0].y=-1; 
		quad.v[1].x=800; quad.v[1].y=-1; 
		quad.v[2].x=800; quad.v[2].y=600; 
		quad.v[3].x=-1; quad.v[3].y=600; 
		
		// Create ball the sprites
		float fps = 30;
		//HTEXTURE tex, int nframes, float FPS, float x, float y, float w, float h
		ballAnims[0] = globals->ballAnims[0]; // TESTING XXXXXXXXXXXXXXXXXX
        ballAnims[1] = globals->ballAnims[1]; // THIS MIGHT CAUSE AN ERROR BECAUSE
		ballAnims[2] = globals->ballAnims[2]; // BALLS A DELETED TWICE
		ballAnims[3] = globals->ballAnims[3];
		ballAnims[4] = globals->ballAnims[4];
		ballAnims[5] = globals->ballAnims[5];
		ballAnims[6] = globals->ballAnims[6];
		
		ballPopAnims[0] = globals->ballPopAnims[0]; // TESTING XXXXXXXXXXXXXXXXXX
        ballPopAnims[1] = globals->ballPopAnims[1]; // THIS MIGHT CAUSE AN ERROR BECAUSE
		ballPopAnims[2] = globals->ballPopAnims[2]; // BALLS A DELETED TWICE
		ballPopAnims[3] = globals->ballPopAnims[3];
		ballPopAnims[4] = globals->ballPopAnims[4];
		ballPopAnims[5] = globals->ballPopAnims[5];
		ballPopAnims[6] = globals->ballPopAnims[6];
		
        baseSpr = globals->baseSpr;
        playerMiddleSpr = globals->playerMiddleSpr;
        playerTopSpr = globals->playerTopSpr;
        recordMiddleSpr = globals->recordMiddleSpr;
        recordTopSpr = globals->recordTopSpr;
		
		//Create the distortion mesh for the back of the victory and game over screens
		mesh = new hgeDistortionMesh(nRows, nCols);
		//mesh->SetBlendMode(BLEND_COLORADD|BLEND_ALPHAADD);
		int n = 0; // REDUNDANT!!! XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		for (int col = 0; col < nRows; col++)
		{
            for (int row = 0; row < nCols; row++)  
            {
        		mesh->SetColor(col, row, 0x00000000);
            }
        }	
		mesh->SetTextureRect(0, 0, 800, 600);
		
		// Load font and create cursor sprite
		
		fnt->SetColor(0xFFFFFFFF);
		cursorSpr = globals->cursorSpr;
		tileSpr = globals->tileSpr; // subject to change
		// Set up the GUI
		gui=new hgeGUI();
        
		gui->AddCtrl(new hgeGUIMenuItem(MENU_START,fnt,snd,menuX,menuY+40,0.0f,butPlay));
		gui->AddCtrl(new hgeGUIMenuItem(MENU_MODE,fnt,snd,menuX,menuY+80,0.1f,butMode));
		gui->AddCtrl(new hgeGUIMenuItem(MENU_SOUND,fnt,snd,menuX,menuY+120,0.2f,butSound));
		gui->AddCtrl(new hgeGUIMenuItem(MENU_MUSIC,fnt,snd,menuX,menuY+160,0.3f,butMusic));
		gui->AddCtrl(new hgeGUIMenuItem(MENU_EXIT,fnt,snd,menuX,menuY+200,0.4f,butExit));

		gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gui->SetFocus(1);
		gui->Enter();
		
		introChannel = hge->Effect_PlayEx(intro, soundVolume, 0, 1.0, true);
		if (!bMusic||!bSound) hge->Channel_Pause(introChannel);
		
		// Start engine
		hge->System_Start();
		
		// Delete created objects and free loaded resources
		delete gui;
		delete fnt;
		delete game;
		delete mesh;
		for (int i = 0; i < sizeof(ballAnims)/sizeof(hgeSprite*); i++)
		{
            delete ballAnims[i];
        }

		hge->Effect_Free(ball_click);
		hge->Effect_Free(intro);
		hge->Effect_Free(snd);
		//hge->Effect_Free(tile_pop);
		//hge->Effect_Free(ball_move);
		//hge->Texture_Free(cursorTex);
		//hge->Texture_Free(quad.tex);
		//hge->Texture_Free(tileTex);
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}
	
	// Restore video mode and free
	// all allocated resources
	hge->System_Shutdown();

	// the HGE object will be deleted.
	hge->Release();
    
	return 0;
}
