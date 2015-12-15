// Game class method implementation

//TO DO:
//-Scoring is a little naive in pop()
//-Implement check for 5 balls in a line
//-Create a more efficient pathfinding algorithm
//Done:
//-Implement findPath() that saves the path or signals that there is no path available
//-Fix "passing through walls" 
//-In findPath: fix the scoring part, observe the currentX and currentY values in the output
//in some cases they were nagative, add marking of tiles with 'f', 'n', 's', 'u'
//-Balls' position on the grid and their x,y coordinates must be clearly separated
//-Balls must be deleted in the destructor
//-I would like to say something like this: If ball at square 1x9 do something
//-Game class should have a start()
//-Game class should hold pointers to all the available ball anims
//-Figure out why spawnBalls varies in ball creation: it might overlay, or misread where there are spaces available
//-A concensus must be reached about how the information about ball presence is held:
// Ball absence is signified by a NULL entry in balls[i][j]

#include "Game.h"
#include "hge.h"
#include "globals.h"
#include "hgeRect.h"
#include "utilities.h"
#include "stdlib.h"

// CONSTANTS
static const int UP = 0;
static const int DOWN = 1;
static const int LEFT = 2;
static const int RIGHT = 3;
static const int STRAIGHT = 4;
static const int SEARCH = 30; // signal values for "action"
static const int BACKTRACK = 31; // '' (had to make the value different to UP, DOWN...)
static const int searchCutOff = 10000;

// TESTING VARIABLES
char* testString = "game Test";
char* strCoordinate = "coordinate";
char* strScores = "scores";
char* strDirX = "X direction";
char* strDirY = "Y direction";
char* strPathX = "path Xs";
char* strPathY = "path Ys";
char* strTex = "ball tex";
char* strLastTex = "last tex";

Game::Game(int _mode, int* _state)
{
    mode = _mode;
    state = state;
    grid = NULL;
    lastPathIndex = 0;
    score = 0;
    // initialize balls
    int w = 9;
    int h = 9;
    int score = 0;
    lineStackLastIndex = -1; // POSSIBLY UNNECESSARY
    selectedBall = NULL;
    ballSelected = false;
    nPredicted = 0;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            balls[i][j] = NULL;
            popGrid[i][j] = false;
        }
    }
    hge = hgeCreate(HGE_VERSION);
}

Game::~Game()
{
    //delete grid
    delete grid;
    //delete gridTile;
    //delete all balls
    //for all balls, delete balls
    int w = grid->getWidth();
    int h = grid->getHeight();
    hge->Release();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (balls[i][j] != NULL) delete balls[i][j];
        }
    }
    //for all ballAnims, delete ballAnims
}

bool Game::start()
{
     //This instantiates the first 3 balls
     //and starts the game
     //for (int n = 0; n < 26; n++) REDUNDANT CODE XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
     startPrediction();
     predictColours();
     predictPositions();
     spawnBalls();
     newTurn();
}

void Game::render() //// Incomplete
{
    if (grid == NULL) //&& balls!=null)
    {
        MessageBox(NULL, "Game not initialized, grid absent","Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    } else
    {
        grid->render();
        
        int w = grid->getWidth(); // remove later
        int h = grid->getHeight();
        //for all balls in balls render
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                if (balls[i][j]) balls[i][j]->render();
            }
        }
    }
    // TESTING OUTPUT
    /*static Globals *globals = Globals::getInstance();
    globals->fnt->printf(200,120, HGETEXT_LEFT, strTex);
    globals->fnt->printf(200,140, HGETEXT_LEFT, strLastTex);
    globals->fnt->printf(200,100, HGETEXT_LEFT, testString);
    globals->fnt->printf(200,120, HGETEXT_LEFT, strCoordinate);
    globals->fnt->printf(200,140, HGETEXT_LEFT, strScores);
    globals->fnt->printf(200,160, HGETEXT_LEFT, strDirX);
    globals->fnt->printf(200,180, HGETEXT_LEFT, strDirY);
    globals->fnt->printf(200,200, HGETEXT_LEFT, strPathX);
    globals->fnt->printf(200,220, HGETEXT_LEFT, strPathY);*/
    
    // END OF TESTING OUTPUT
    
    // if mode 2 show balls to be spawned
    // if mode 3 show balls and their future locations
}


void Game::update(float dt) ///// incomplete
{
    // update balls
    static int w = grid->getWidth();
    static int h = grid->getHeight();
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (balls[i][j]) balls[i][j]->update(dt);
        }
    }
          
    // update grid
    grid->update(dt);
}

bool Game::onClickBall()
{
    // check if a tile containing a Ball has been clicked
    int w = 0;
    int h = 0;
    //onClickTile(&w,&h); MIGHT BE USEFUL WWWWWWWWWWWWWWWWWWWWWWWWWWWWW
    if (onClickTile(&w,&h))
    {
        if (balls[w][h] != NULL)
        {
            // unselect old ball
            if (ballSelected != 0) balls[selectedBallX][selectedBallY]->unselect();
            ballSelected = true;
            selectedBallX = w;
            selectedBallY = h;
            balls[selectedBallX][selectedBallY]->select();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Game::onClickTile(int *row, int *column)
{
    // check all the tiles in the grid
    // return the first tile ran responds should be the only tile
    static int gridW = grid->getWidth();
    static int gridH = grid->getHeight();
    static Globals* globals = Globals::getInstance();
    
    for (int w = 0; w < gridW; w++)
    {
        for (int h = 0; h < gridH; h++)
        {
            // test the mouse point
            // test for key up
            if (grid->getRect(w,h)->TestPoint(globals->mouseX, globals->mouseY) &&
               hge->Input_KeyUp(HGEK_LBUTTON))
            {
                *row = w;
                *column = h;
                tileSelected = true;
                selectedTileX = w;
                selectedTileY = h;
                grid->highlight(w,h,true);
                return true;
            }
        }
    }
    return false;       
}

Grid* Game::getGrid() //// DONE!
{
    return grid;
}

void Game::setGrid(int w, int h, hgeSprite* gt) // DONE!
{
    width = w;
    height = h;
    grid = new Grid(w,h,gt);
}

/*bool Game::spawnBalls() //Done - as far as I know
{
    // Randomly try to spawn 3 balls on the playing field
    //find available spot in balls[][]
    //spawn ball there     
    //i.e. 
    //1.get all available
    //2.randomly select 1
    //3.create ball of random colour
    //4.repeat another 1-3 2 times
    static int gridW = grid->getWidth();
    static int gridH = grid->getHeight();
    bool spaceAvailable; //// ADHOC!
    int index;
    int availableWs[gridW*gridH];
    int availableHs[gridH*gridW];
    int nSpaces;
    int theW;
    int theH;
    static int ballsToSpawn = 3;
    float x, y;
    // Spawn ball
    for (int n = 0; n < ballsToSpawn; n++) /// TEST!!!! do just once
    {
        // Set variables up for pass n
        nSpaces = 0;
        spaceAvailable = false;
        // Search for an available spaces
        for (int w = 0; w < gridW; w++)
        {
            for (int h = 0; h < gridH; h++)
            {
                if (balls[w][h] == NULL)
                {
                    // store the available space
                    availableWs[nSpaces] = w;
                    availableHs[nSpaces] = h;
                    nSpaces++;
                    spaceAvailable = true;
                }
            }
        }
        
        // If there are spaces spawn a Ball
        if (spaceAvailable)
        {
            // randomly pick a space of those available
            // for w
            index = hge->Random_Int(0, nSpaces-1); // Might need to do nSpaces - 1 - DAMN STRAIGHT!
            theW = availableWs[index];
            theH = availableHs[index];
            // create Ball object
            x = grid->getTileX(theW);
            y = grid->getTileY(theH);
            balls[theW][theH] = new Ball(x,y,getRandomColour());
        }   
        // Check for game over condition - when no spaces left to spaw even first ball;
        //if (!spaceAvailable && n == 0) return true; 
    }
    return false;
}*/

bool Game::spawnBalls()
{
    // Spawn 3 balls of 3 colours in 3 locations
    for (int i = 0; i < nPredicted; i++)
    {
        if (balls[predX[i]][predY[i]] == NULL)
        {
            createBall(predIndex[i], predX[i], predY[i]);//balls[predX[i]][predY[i]] = new Ball(grid->getTileX(predX[i]),grid->getTileY(predY[i]),ballPred[i]);
        } else
        {
            // Find a new location
            predictPosition();
            createBall(predIndex[i], predX[0], predY[0]);//balls[predX[0]][predY[0]] = new Ball(grid->getTileX(predX[0]),grid->getTileY(predY[0]),ballPred[i]);
        }
    }
    if (nPredicted < 3)
    {
        nPredicted = 0;
        return true;
    } else
    {
        nPredicted = 0;
        return false;
    }
}

hgeAnimation* Game::getRandomColour()
{
    // pick number between 0 and nBalls(-1)
    static Globals* globals = Globals::getInstance();
    int randomIndex = globals->hge->Random_Int(0,6); //something random
    return ballAnims[randomIndex];
}

int Game::getRandomColourIndex()
{
    // pick number between 0 and nBalls(-1)
    static Globals* globals = Globals::getInstance();
    int randomIndex = globals->hge->Random_Int(0,6); //something random
    return randomIndex;
}

void Game::setBalls(hgeAnimation *anims[], hgeAnimation *popAnims[], int _nBalls)
{
// Should work like this: posX, posY are supplied and their position is worked out to
// be that of a cell on the grid. I.e. grid->translatePosition()
   //
    nBalls = _nBalls;
    for (int i = 0; i< nBalls; i++)
    {
        ballAnims[i] = anims[i];
        ballPopAnims[i] = popAnims[i]; 
    }
}

bool Game::findPath()
{
    int gW = grid->getWidth();
    int gH = grid->getHeight();
    // Construct a char array
    // v signifying has this tile should be used
    // s - selected
    // n - this tile should not be used
    // u - this tile has not been visited
    char T[gW][gH];
    // nullify all entries in array
    for (int w = 0; w < gW; w++)
    {
        for (int h = 0; h < gH; h++)
        {
         T[w][h] = 'u';
        }
    }
    
    // Work out the direction in which to start the search first
    // Direction is either right, left, up or down.
    
    int directionX = 0;
    int directionY = 0;
    
    // Def. tile A is a neigbour to B iff. - |(A.x - B.x)| >= 1 and |(A.y - B.y)| >= 1 FIX THIS!
    
    // Def. a path from A to B is an array of unoccupied tiles T s.t. 
    // any one tile T[i] has atleast 1 neighbour
    
    // Smarter search - Depth First Search
    
    // Search variables
    
    int scores[] = {0,0,0,0}; // scoress are 0 in the beginning
    
    int action = SEARCH; // if you reach a deadend backtrack
    
    int currentX = selectedBallX;
    int currentY = selectedBallY;
    int nextX = 0; // maybe redundant
    int nextY = 0; // this too
    int lastIndex = 0;
    
    // Add the first tile to be part of the path
    pathX[lastIndex] = selectedBallX;
    pathY[lastIndex] = selectedBallY;
    
    // enter the loop
    for (int n = 0; n < searchCutOff; n++) //gW*gH
    {        
        switch (action)
        {
            case SEARCH: // search and possibly add a new tile
            {
                // Grade the choices: scores goes from 0 to 3
                // To choose the next tile to select grade each choice
                // and choose the highest.
                // choices can be up, down, left, right
                // unless the current tile is against the board edge
                
                // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                /*static char strCoor1[100];
                static char strCoor2[100];
                itoa(currentX, strCoor1, 10);
                itoa(currentY, strCoor2, 10);
                concat(strCoor1, " ");
                concat(strCoor1, strCoor2);
                strCoordinate = strCoor1;*/
                
                // find the direction
                if (currentX-selectedTileX > 0)
                {
                    directionX = LEFT;
                    //strDirX = "left"; 
                } else if (currentX-selectedTileX < 0)
                {
                    directionX = RIGHT;
                    //strDirX = "right"; 
                } else
                {
                    directionX = STRAIGHT;
                }
                
                if (currentY-selectedTileY > 0)
                {
                    directionY = UP;
                    //strDirY = "up"; 
                } else if (currentY-selectedTileY < 0)
                {
                    directionY = DOWN;
                    //strDirY = "down"; 
                } else
                {
                    directionY = STRAIGHT;
                }
                    
                // for up currentX, currentY-1    
                // if occupied - mark as 'n'
                // unexistant or finished scores is zero
                if (balls[currentX][currentY-1] != NULL || T[currentX][currentY-1] == 'n' || currentY-1 < 0)
                {
                    scores[UP] = 0;
                } else // it matches the direction of the target tile 
                {
                    if (UP == directionY) scores[UP] += 1;
                    if (T[currentX][currentY-1] == 'u') scores[UP] += 1;
                    if (currentX == selectedTileX && currentY-1 == selectedTileY) scores[UP] += 1;
                }
                
                // for down currentX, currentY+1
                if (balls[currentX][currentY+1] != NULL || T[currentX][currentY+1] == 'n' || currentY+1 >= gH)
                {
                    scores[DOWN] = 0;
                } else
                {
                    if (DOWN == directionY) scores[DOWN] += 1;
                    if (T[currentX][currentY+1] == 'u') scores[DOWN] += 1;
                    if (currentX == selectedTileX && currentY+1 == selectedTileY) scores[DOWN] += 1;
                }
                
                // for left currentX-1, currentY
                if (balls[currentX-1][currentY] != NULL || T[currentX-1][currentY] == 'n' || currentX-1 < 0)
                {
                    scores[LEFT] = 0;
                } else 
                {
                    if (LEFT == directionX) scores[LEFT] += 1;
                    if (T[currentX-1][currentY] == 'u') scores[LEFT] += 1;
                    if (currentX-1 == selectedTileX && currentY == selectedTileY) scores[LEFT] += 1; 
                }
                
                // for right currentX+1, currentY
                if (balls[currentX+1][currentY] != NULL || T[currentX+1][currentY] == 'n' || currentX+1 >= gW)
                {
                    scores[RIGHT] = 0;
                } else
                {
                    if (RIGHT == directionX) scores[RIGHT] += 1;
                    if (T[currentX+1][currentY] == 'u') scores[RIGHT] += 1;
                    if (currentX+1 == selectedTileX && currentY == selectedTileY) scores[RIGHT] += 1;
                }
                
                // TESTING xxxxxxxxxxxxxxxxxxxxxxxxx
                //static char strArray[10];
                //toString(scores, 4, strArray);
                //strScores = strArray;
                
                /*static char str2[100];
                itoa(maxIndex(scores, 4), str2, 10);
                testString = str2;*/ //TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                
                // check for loops                
                // check for a dead-end
                if ((scores[UP]+scores[DOWN]+scores[LEFT]+scores[RIGHT]) == 0)
                {
                    T[currentX][currentY] = 'n';
                    action = BACKTRACK;
                    
                    // if the first tile runs out of options quit the search
                    if (selectedBallX == currentX && selectedBallY == currentY)
                    {
                        return false;
                    }
                    
                    break;
                } else
                {
                    // continue
                }
                
                // choose next tile
                // that being the tile with the highest scores 
                switch(maxIndex(scores, 4)) // provided this works
                {
                    case (UP):
                        // set nextX, nextY
                        nextX = currentX;
                        nextY = currentY-1;
                        break;
                    case (DOWN):
                         // set nextX, nextY
                         nextX = currentX;
                         nextY = currentY+1;
                         break;
                    case (LEFT):
                         // set nextX, nextY
                         nextX = currentX-1;
                         nextY = currentY;
                         break;
                    case (RIGHT):
                         // set nextX, nextY
                         nextX = currentX+1;
                         nextY = currentY;
                         break;
                    default:
                         // set nextX, nextY
                         break; // break out of SEARCH and loop around again
                         //nextX = currentX+1;
//                         nextY = currentY;
                }
                
                //annulate scores
                scores[UP] = 0;
                scores[DOWN] = 0;                
                scores[LEFT] = 0;                
                scores[RIGHT] = 0;
                
                //testString = "searched";
                
                // Set this as the current tile
                currentX = nextX;
                currentY = nextY;
                
                //wait(); TESTING DOESN'T WORK AS IT SHOULD
                
                /*static char str1[100];
                static char str2[100];
                itoa(currentX, str1, 10);
                itoa(currentY, str2, 10);
                concat(str1, " ");
                concat(str1, str2);
                testString = str1;*/
                
                // Save the last tile for backtracking
                lastIndex++;
                pathX[lastIndex] = currentX;
                pathY[lastIndex] = currentY;
                
                // Mark this as selected
                T[currentX][currentY] = 's';
                
                // if the desired tile has been reached quit search
                if (currentX == selectedTileX && currentY == selectedTileY)
                {
                    lastPathIndex = lastIndex;
                    
                    // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx
                    // show the path Xs and Ys
                    //static char strPX[30];
//                    static char strPY[30];
//                    toString(pathX, lastPathIndex, strPX);
//                    toString(pathY, lastPathIndex, strPY);
//                    strPathX = strPX;
//                    strPathY = strPY;
                    
                    return true; // path has been found
                }
                break;
            }
            case BACKTRACK:
            {
                // set the current tile as the last
                lastIndex--; // set the index to point to the previous value
                currentX = pathX[lastIndex];
                currentY = pathY[lastIndex];
                action = SEARCH;
                testString = "backtracked";
                // loop around
                break;
            }
        }
    }    
    
    //static char strPX[30];
//    static char strPY[30];
//    toString(pathX, lastPathIndex, strPX);
//    toString(pathY, lastPathIndex, strPY);
//    strPathX = strPX;
//    strPathY = strPY;
    
    return false; // no path has been found
}

void Game::highlightPath()
{
    // this function highlights the individual tiles
    // as specified by the pathX and pathY arrays
    for (int n=0; n < lastPathIndex; n++)
    {
        // highlight this tile, although this is a special highlight
        // as it highlights only for a small time
        grid->specialHighlight(pathX[n], pathY[n], n*2);
    }
}

bool Game::neighbours(int x1, int y1, int x2, int y2)
{
    // either x1 == x2 and difference in ys is 1
    // or the other way around
    if (x1 == x2 && (makePositive(y1 - y2) == 1))
    {
        return true;
    }
    
    // or...
    if (y1 == y2 && (makePositive(x1 - x2) == 1))
    {
        return true;
    }
    return false;
}

void Game::moveBall()
{
     // schedule the selected ball to move
     // either following a path
     // or after the tile animation
     // finished
     
     // for now move ball instantaneously
     if (ballSelected)
     {
         balls[selectedTileX][selectedTileY] = balls[selectedBallX][selectedBallY];
         balls[selectedTileX][selectedTileY]->setPosition(grid->getTileX(selectedTileX), 
         grid->getTileY(selectedTileY));
     }
     
     // empty the previous location
     balls[selectedBallX][selectedBallY] = NULL;
     
     // deselect currentBall;
     if (selectedBall != NULL) selectedBall->unselect();
     selectedBall = NULL;
     if (ballSelected) balls[selectedTileX][selectedTileY]->unselect();
     ballSelected = false;

    // deselect the tile
    tileSelected = false;

    // wipe clean the path variables
    lastPathIndex = -1;
}

int Game::pop()
{
    int points = 0;
    // Play popping animation of all the balls in popGrid
    for (int w = 0; w < grid->getWidth(); w++)
    {
        for (int h = 0; h < grid->getHeight(); h++)
        {
            if (popGrid[w][h] && balls[w][h] != NULL)
            {
                points++; // Scoring is a little naive
                balls[w][h]->pop(); //Should be popping those balls
            }
            //clear popGrid
            //popGrid[w][h] = false; // THIS IS NOW DONE IN popped()
        }
    }
    
    
    // Calculate the scores yielded from the popping
    score += points;
    return points;
}

int Game::getLineStackHeight()
{
    return lineStackLastIndex+1;
}

void Game::clearLineStack()
{
    lineStackLastIndex = -1;
}

void Game::saveLineStack()
{
    int gW = grid->getWidth();
    int gH = grid->getHeight(); // REDUNTANT
    
    // save the line stack to popGrid
    for (int i = 0; i <= lineStackLastIndex; i++)
    {
        popGrid[lineStackX[i]][lineStackY[i]] = true;
    }
    // clear the line stack
    clearLineStack();
}

bool Game::continuesLineStack(Ball* ball)
{
    /*static char strTex1[10]; // TESTING XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    itoa(lineStackTextureID, strTex1, 10);
    strTex = strTex1;
    static char strTex2[10];
    itoa(ball->getTextureID(), strTex2, 10);
    strLastTex = strTex2;*/
    if (lineStackTextureID == ball->getTextureID())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Game::addToLineStack(int w, int h, int texID)
{ 
    lineStackTextureID = texID;
    lineStackLastIndex++;
    lineStackX[lineStackLastIndex] = w;
    lineStackY[lineStackLastIndex] = h; // THIS WILL GET ADDED MANY TIMES, COULD ADD startNewLineStack
}

bool Game::findCompleteLine(int w, int h)
{
    static int nBoundary = 5;
    if (getLineStackHeight() == 0)
    {
        if (balls[w][h] != NULL) // you've just met a new potential line
        {
            addToLineStack(w,h,balls[w][h]->getTextureID());
        } else
        {
            if (getLineStackHeight() < nBoundary) clearLineStack();
        }
    } else if (getLineStackHeight() > 0)
    {
        if (balls[w][h] != NULL)
        {
            // if colour matches the colour of the previous ball
            // register this ball increment 
            if (continuesLineStack(balls[w][h])) // THIS DOESN'T WORK
            {
                addToLineStack(w,h,balls[w][h]->getTextureID());
            } else // you have encountered a ball of a different colour: start a new line
            {
                if (getLineStackHeight() < nBoundary) 
                {
                    clearLineStack();
                    addToLineStack(w,h,balls[w][h]->getTextureID());
                } else
                {
                    return true; // the line has ended, no more can fit on this row
                }
            }
        } else
        {
            if (getLineStackHeight() < nBoundary) 
            {
                clearLineStack();
            } else
            {
                return true;
            }
        }
    }
    return false;
}

bool Game::findCompleteLines()
{
    // If there is atleast one complete line of 5 balls record it and return true
    // Save the balls in popGrid
    // else return false
    
    int gW = grid->getWidth();
    int gH = grid->getHeight();
    
    bool completeLines = false;
    static int nBoundary = 5;
    
    // Horizontal lines
    for (int h = 0; h < gH; h++)
    {
        for (int w = 0; w < gW; w++)
        {
            if (findCompleteLine(w, h)) break;
        }
        // If the lineCount >= 5, add the balls to the popArray
        if (getLineStackHeight() >= nBoundary)
        {
            completeLines = true;
            saveLineStack();
        } else
        {
            clearLineStack();
        }
    }
    
    // Vertical lines
    for (int h = 0; h < gH; h++)
    {
        for (int w = 0; w < gW; w++)
        {
            if (findCompleteLine(h, w)) break;
        }
        // If the lineCount >= 5, add the balls to the popArray
        if (getLineStackHeight() >= nBoundary)
        {
            completeLines = true;
            saveLineStack();
        } else
        {
            clearLineStack();
        }
    }
    // Left diagonal
    
    int nLines = 9;
    int lineLengths[] = {5,6,7,8,9,8,7,6,5};
    int wInits[] = {0,0,0,0,0,1,2,3,4};
    int hInits[] = {4,3,2,1,0,0,0,0,0};
    
    for (int nSeqs = 0; nSeqs < nLines; nSeqs++)
    {
        int w = wInits[nSeqs];
        int h = hInits[nSeqs];
        for (int nLen = 0; nLen < lineLengths[nSeqs]; nLen++)
        {
            if (findCompleteLine(w, h)) break;
            w++;
            h++;
        }
        // If the lineCount >= 5, add the balls to the popArray
        if (getLineStackHeight() >= nBoundary)
        {
            completeLines = true;
            saveLineStack();
        } else
        {
            clearLineStack();
        }
    }
    
    // Right diagonal
    
    int wInits2[] = {0,0,0,0,0,1,2,3,4};
    int hInits2[] = {4,5,6,7,8,8,8,8,8};
    
    for (int nSeqs = 0; nSeqs < nLines; nSeqs++)
    {
        int w = wInits2[nSeqs];
        int h = hInits2[nSeqs];
        for (int nLen = 0; nLen < lineLengths[nSeqs]; nLen++)
        {
            if (findCompleteLine(w, h)) break;
            w++;
            h--;
        }
        // If the lineCount >= 5, add the balls to the popArray
        if (getLineStackHeight() >= nBoundary)
        {
            completeLines = true;
            saveLineStack();
        } else
        {
            clearLineStack();
        }
    }
    
    if (completeLines)
    {
        return true;
    } else
    {
        return false;
    }
}

bool Game::checkNewRecord()
{
    // Look up the record file
    // If the new score is a new record
    // return true
    // else return false
    Globals* globals = Globals::getInstance();
    if (score > globals->recordScore)
    {
        return true;
    } else
    {   
        return false;
    }
}

bool Game::popped()
{
    bool popped = true;
    for (int w = 0; w < grid->getWidth(); w++)
    {
        for (int h = 0; h < grid->getHeight(); h++)
        {
            if (balls[w][h]) 
                if (!balls[w][h]->popped() && popGrid[w][h]) 
                    popped = false;
        }
    }
    
    if (popped)
    {
        for (int w = 0; w < grid->getWidth(); w++)
        {
            for (int h = 0; h < grid->getHeight(); h++)
            {
                if (popGrid[w][h] && balls[w][h] != NULL)
                {
                    balls[w][h]->unselect();
                    if (selectedBall != NULL) selectedBall = NULL; // this shouldn't be necessary
                    delete balls[w][h];
                    balls[w][h] = NULL;            
                }
                //clear popGrid
                popGrid[w][h] = false;
            }
        }
        return true;
    } else
    {      
        return false;
    }
}

bool Game::gridIsFull()
{
    for (int w = 0; w < grid->getWidth(); w++)
    {
        for (int h = 0; h < grid->getHeight(); h++)
        {
            if (balls[w][h] == NULL)
            {
                return false;
            }
        }
    }
    return true;
}

int Game::getScore()
{
    return score;
}

float Game::getPredX(int i)
{
    return grid->getTileX(predX[i]);
}

float Game::getPredY(int i)
{
    return grid->getTileY(predY[i]);
}

void Game::predictColours()
{
    Globals* globals = Globals::getInstance();
    // Set the current prediction to the forward prediction
        for (int i = 0; i < 3; i++)
        {
            if (ballPred[i]) ballPred[i] = NULL;
            predIndex[i] = predIndex2[i];
            ballPred[i] = ballAnims[predIndex[i]];
        }
    // Make a further prediction
    for (int i = 0; i < 3; i++)
    {
        if (ballPred2[i]) ballPred2[i] = NULL;
        predIndex2[i] = getRandomColourIndex();
        ballPred2[i] = ballAnims[predIndex2[i]];
    }
}

void Game::startPrediction()
{
    for (int i = 0; i < 3; i++)
    {
        predIndex2[i] = getRandomColourIndex();
    }
}

void Game::predictPositions()
{
    // Randomly try to spawn 3 balls on the playing field
    //find available spot in balls[][]
    //spawn ball there     
    //i.e. 
    //1.get all available
    //2.randomly select 1
    //3.create ball of random colour
    //4.repeat another 1-3 2 times
    static int gridW = grid->getWidth();
    static int gridH = grid->getHeight();
    bool spaceAvailable; //// ADHOC!
    int index;
    int availableWs[gridW*gridH];
    int availableHs[gridH*gridW];
    int nSpaces;
    int theW;
    int theH;
    static int ballsToSpawn = 3;
    float x, y;
    nPredicted = 0;
    bool availGrid[gridW][gridH];
    for (int w = 0; w < gridW; w++)
    {
        for (int h = 0; h < gridH; h++)
        {
            if (balls[w][h] == NULL)
            {
                availGrid[w][h] = true;
            } else
            {
                availGrid[w][h] = false;
            }
        }
    }    
    
    // Spawn ball
    for (int n = 0; n < ballsToSpawn; n++) /// TEST!!!! do just once
    {
        // Set variables up for pass n
        nSpaces = 0;
        spaceAvailable = false;
        // Search for an available spaces
        for (int w = 0; w < gridW; w++)
        {
            for (int h = 0; h < gridH; h++)
            {
                if (availGrid[w][h] == true)
                {
                    // store the available space
                    availableWs[nSpaces] = w;
                    availableHs[nSpaces] = h;
                    nSpaces++;
                    spaceAvailable = true;
                }
            }
        }
        
        // If there are spaces spawn a Ball
        if (spaceAvailable)
        {
            // randomly pick a space of those available
            // for w
            index = hge->Random_Int(0, nSpaces-1); // Might need to do nSpaces - 1 - DAMN STRAIGHT!
            theW = availableWs[index];
            theH = availableHs[index];
            // Save prediction
            predX[n] = theW;
            predY[n] = theH;
            availGrid[theW][theH] = false;
            nPredicted++;
        }
    }   
}

void Game::predictPosition()
{
    static int gridW = grid->getWidth();
    static int gridH = grid->getHeight();
    bool spaceAvailable; //// ADHOC!
    int index;
    int availableWs[gridW*gridH];
    int availableHs[gridH*gridW];
    int nSpaces;
    int theW;
    int theH;
    static int ballsToSpawn = 1;
    float x, y;
    bool availableGrid[gridW][gridH];
    for (int w = 0; w < gridW; w++)
    {
        for (int h = 0; h < gridH; h++)
        {
            if (balls[w][h] == NULL)
            {
                availableGrid[w][h] = true;
            }
        }
    }    
    
    // Spawn ball
    for (int n = 0; n < ballsToSpawn; n++) /// VERY ADHOC
    {
        // Set variables up for pass n
        nSpaces = 0;
        spaceAvailable = false;
        // Search for an available spaces
        for (int w = 0; w < gridW; w++)
        {
            for (int h = 0; h < gridH; h++)
            {
                if (availableGrid[w][h] == true)
                {
                    // store the available space
                    availableWs[nSpaces] = w;
                    availableHs[nSpaces] = h;
                    nSpaces++;
                    spaceAvailable = true;
                }
            }
        }
        
        // If there are spaces spawn a Ball
        if (spaceAvailable)
        {
            // randomly pick a space of those available
            // for w
            index = hge->Random_Int(0, nSpaces-1); // Might need to do nSpaces - 1 - DAMN STRAIGHT!
            theW = availableWs[index];
            theH = availableHs[index];
            // Save prediction
            predX[n] = theW;
            predY[n] = theH;
            availableGrid[theW][theH] = false;
        }
    }   
}

void Game::newTurn()
{
    predictColours();
    predictPositions();
}

void Game::createBall(int ballIndex, int x, int y)
{
    Globals *globals = Globals::getInstance();
    balls[x][y] = new Ball(grid->getTileX(x),grid->getTileY(y),ballAnims[ballIndex], ballPopAnims[ballIndex]); 
    balls[x][y]->setSound(globals->notes[ballIndex]);    
}

bool Game::finishedSpecialHighlight()
{
    // check the grid if all specialHighlightGrid entries are false, return true, else false
    return grid->finishedSpecialHighlight();
}
