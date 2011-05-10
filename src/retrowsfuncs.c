#include <stdio.h>

#include "retrowsfuncs.h"

/*
    Tries to move the sprite closer and closer to x/y
    Returns 0 if colission never occured.
    Returns 1 if colission happened in the end.
*/
int RWS_MoveCheckWorldCol(RetroSprite_t *s, RetroWorldScreen_t *w, int x, int y)
{
    int xv, yv;
    int xp, yp;
    int xft, xlt;
    int yft, ylt;

    while ( s->x != x || s->y != y )
    {
        if ( s->x > x )
            { xv = -1; }
        else if ( s->x < x )
            { xv = 1; }
        else
            { xv = 0; }

        if ( s->y > y )
            { yv = -1; }
        else if ( s->y < y )
            { yv = 1; }
        else
            { yv = 0; }

        xft = (s->x + xv + s->spr->anims[s->anim].box.x ) / w->tw;
        xlt = (s->x + xv + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w - 1) / w->tw;
        yft = (s->y + yv + s->spr->anims[s->anim].box.y) / w->th;
        ylt = (s->y + yv + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h - 1) / w->th;

        for(yp=yft;yp<=ylt;yp++)
        {
            for(xp=xft;xp<=xlt;xp++)
            {

                switch(w->colmap[yp*w->w + xp])
                {
                    case RW_COL_NONE:
                        continue;
                        break;
                    case RW_COL_SOLID:
                        return 1;
                        break;
                    default:
                        printf("Something not none and not solid --\n");
                        break;
                }

            }
        }
        s->x += xv;
        s->y += yv;
    }
    return 0;
}

