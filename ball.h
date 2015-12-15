// This is the class of the ball in the game
// It renders and updates itself

#ifndef BALL
#define BALL

#include "hgeAnim.h"
#include "globals.h"

class Ball
{
    public:
        Ball(float x, float y, hgeAnimation* anim, hgeAnimation* popAnim);
        ~Ball();
        void render();
        void update(float dt);
        void setPosition(float x, float y);
        void getPosition();
        void unselect();
        void select();
        void pop();
        void setSound(HEFFECT sound);
        bool popped();
        HTEXTURE getTextureID();
        
    private:
        int POPPING;
        int POPPED;
        int NORMAL;
        Globals* globals;
        int posX;
        int posY;
        int state;
        bool selected;
        float x;
        float y;
        float rot;
        float hscale;
        float vscale;
        hgeAnimation* anim; // the graphic to be rendered for this ball
        hgeAnimation* popAnim;
        HEFFECT sound;
};

#endif
