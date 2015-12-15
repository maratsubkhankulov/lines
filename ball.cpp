// Implementation of the Ball class

#include "ball.h"
#include "hgeAnim.h"
#include "hge.h"
#include <math.h>
#include "globals.h"
#include "utilities.h"

Ball::Ball(float _x, float _y, hgeAnimation *_anim, hgeAnimation *_popAnim)
{
    globals = Globals::getInstance();
    selected = false;
    x = _x;
    y = _y;
    anim = new hgeAnimation(*_anim);
    popAnim = new hgeAnimation(*_popAnim);
    rot = 0;
    POPPING = 0;
    POPPED = 1;
    NORMAL = 2;
    state = NORMAL;
    float gridSizeX = 500;
    float gridSizeY = 500;
    float texHeight = 60;
    float texWidth = 600;
    int frames = anim->GetFrames();
    float frameWidth = texWidth/frames;
    float frameHeight = texHeight;
    int gridDimX = 9;
    int gridDimY = 9;
    float ballWidth = gridSizeX/9;
    float ballHeight = gridSizeY/9;
    hscale = ballWidth/frameWidth;
    vscale = ballHeight/frameHeight;
    anim->Play();
}

Ball::~Ball()
{
    delete anim;
    //globals->hge->Effect_Free(sound);
}

void Ball::render()
{
    if (state == NORMAL) 
        anim->RenderEx(x, y, rot, hscale, vscale);
            else popAnim->RenderEx(x, y, rot, hscale, vscale);
}

void Ball::update(float dt)
{
// on it's state
// and possibly the system's state too
//     bounce/update animation
//     change colour
    static float offsetX = 0;
    static float offsetY = 0;
    
    Globals* globals = Globals::getInstance();
    
    if (selected)
    {
        //bounce
        /*offsetX += globals->ballRockSpeed*dt;
        offsetY += globals->ballRockSpeed*dt;
        x += (sin(offsetX))*globals->ballRockX;
        y += (cos(offsetY))*globals->ballRockY;*/
        //animate the graphic
        anim->Update(dt);
    }
    
    if (state == POPPING)
    {
        popAnim->Update(dt);
        if (popAnim->IsPlaying() == false) state = POPPED;
    }
}

void playSound(HEFFECT sound);

void Ball::select()
{
    playSound(sound);
    selected = true;
}

void Ball::unselect()
{
    selected = false;
}

void Ball::setPosition(float _x, float _y)
{
    x = _x;
    y = _y;
}

void Ball::pop()
{
    // Play the popping animation
    // might spawn errors
    state = POPPING;
    popAnim->Play();
}

HTEXTURE Ball::getTextureID()
{
    float x, y, w, h;
    anim->GetTextureRect(&x, &y, &w, &h);
    float id = x+y*10+w*100+h*1000;
    return (int)id;
}

bool Ball::popped()
{
    if (state == POPPED)
    {
        state = NORMAL;
        return true;
    }
    return false;
}

void Ball::setSound(HEFFECT snd)
{
    sound = snd;
}
