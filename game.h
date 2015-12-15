// The lines game class
// To make this work:
// Construct Game()
// setGrid()
// setBalls()
// start()
// call render and update from frame and render functions()

#include "ball.h"
#include "grid.h"
#include "hgeSprite.h"
#include "hgeAnim.h"
#include "hge.h"
#include "hgeRect.h"

class Game
{     
    public:
        Game(int mode, int* state); // start game under a give mode
        ~Game();
        bool start();
        void pause(); // questionable
        void gameOver(); // unimplemented
        void resume();
        void render();
        void update(float dt);
        void setBalls(hgeAnimation* anim[7], hgeAnimation *popAnims[7], int nBalls);
        void setGrid(int width, int height, hgeSprite* gridTile);
        bool spawnBalls(); // if all setting fails, game is over ADHOC!
        bool onClickBall();
        bool onClickTile(int *w, int *h);
        bool findPath();
        void moveBall();
        hgeAnimation* getRandomColour(); // ADHOC!
        int getRandomColourIndex();
        Grid* getGrid();// reduntant in the future
        void highlightPath();
        int pop();
        bool findCompleteLines();
        bool checkNewRecord();
        bool popped();
        bool gridIsFull();
        int getScore();
        float getPredX(int i);
        float getPredY(int i);
        void predictColours();
        void predictPositions();
        void predictPosition();
        void startPrediction();
        void newTurn();
        void createBall(int ballIndex, int x, int y);
        bool finishedSpecialHighlight();
        
        hgeAnimation* ballPred[3];
        hgeAnimation* ballPred2[3];
        int predX[3];
        int predY[3];
        int predIndex[3];
        int predIndex2[3];
        int nPredicted;

    private:
            
        HGE *hge;
        Ball* balls[9][9];
        bool popGrid[9][9];
        int lineStackX[9];
        int lineStackLastIndex;
        int lineStackY[9];
        int lineStackTextureID;
        int pathX[10000];
        int pathY[10000];
        int lastPathIndex;
        Ball *selectedBall;
        bool tileSelected;
        bool ballSelected;
        int selectedBallX;
        int selectedBallY;
        int selectedTileX;
        int selectedTileY;
        Grid* grid;
        int width;
        int height;
        int mode;
        int nBalls;
        int *state;
        hgeSprite *gridTile;
        hgeAnimation *ballAnims[7];
        hgeAnimation *ballPopAnims[7];
        int score;
        
        bool neighbours(int x1, int y1, int x2, int y2);
        void addToLineStack(int w, int h, int texID);
        int getLineStackHeight();
        void saveLineStack();
        void clearLineStack();
        bool continuesLineStack(Ball* ball);
        bool findCompleteLine(int w, int h);
};
    
        
