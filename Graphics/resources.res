Texture ballTex1
{
 filename = "Graphics\ballTex1.png"
 resgroup = 0
}

Sprite testSprite ; TEST XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{
 texture = ballTex1
 rect = 1,1,60,60
 resgroup = 0
}

Animation ballAnim1
{
 texture = ballTex1
 rect = 1,1,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim2
{
 texture = ballTex1
 rect = 1,62,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim3
{
 texture = ballTex1
 rect = 1,123,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim4
{
 texture = ballTex1
 rect = 1,184,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim5
{
 texture = ballTex1
 rect = 1,245,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim6
{
 texture = ballTex1
 rect = 1,306,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballAnim7
{
 texture = ballTex1
 rect = 1,367,60,60
 frames=10
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim1
{
 texture = ballTex1
 rect = 1,428,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim2
{
 texture = ballTex1
 rect = 1,489,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim3
{
 texture = ballTex1
 rect = 1,550,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim4
{
 texture = ballTex1
 rect = 1,611,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim5
{
 texture = ballTex1
 rect = 1,672,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim6
{
 texture = ballTex1
 rect = 1,733,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

Animation ballPopAnim7
{
 texture = ballTex1
 rect = 1,794,60,60
 frames=10
 fps = 40.0
 mode=FORWARD,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup = 0
}

; Statue object

Texture statue1
{
 filename = "Graphics/statue1.png"
 resgroup = 0
}

Sprite playerTop
{
 texture = statue1
 rect = 1,1,120,204
 resgroup = 0
}

Sprite recordTop
{
 texture = statue1
 rect = 123,1,120,198
 resgroup = 0
}

Sprite base
{
 texture = statue1
 rect = 1,206,120,38
 resgroup = 0
}

Sprite middleSection
{
 texture = statue1
 rect = 121,200,120,7
 resgroup = 0
}

; Other Graphics

Texture other1
{
 filename = "Graphics/other1.png"
 resgroup = 0
}

Sprite bg
{
 texture = other1
 rect = 1,1,800,600
 resgroup = 0
}

Sprite tile
{
 texture = other1
 rect = 1,602,60,60
 resgroup = 0
}

Sprite cursor
{
 texture = other1
 rect = 802,1,32,32
 resgroup = 0
}




