#include <string.h>
#include <SDL_opengl.h>

#include "retrolights.h"


static float *vcoords = NULL;
static float *ccoords = NULL;

static inline void RL_SetTileColor(RetroWorldScreen_t *w, int x, int y, float r, float g, float b)
{
    int first = (y * w->w) + x;
    first *= 12;

    if ( ccoords[first+0] < r ) ccoords[first+0] = r;
    if ( ccoords[first+1] < g ) ccoords[first+1] = g;
    if ( ccoords[first+2] < b ) ccoords[first+2] = b;

    if ( ccoords[first+3] < r ) ccoords[first+3] = r;
    if ( ccoords[first+4] < g ) ccoords[first+4] = g;
    if ( ccoords[first+5] < b ) ccoords[first+5] = b;

    if ( ccoords[first+6] < r ) ccoords[first+6] = r;
    if ( ccoords[first+7] < g ) ccoords[first+7] = g;
    if ( ccoords[first+8] < b ) ccoords[first+8] = b;

    if ( ccoords[first+9] < r ) ccoords[first+9] = r;
    if ( ccoords[first+10] < g ) ccoords[first+10] = g;
    if ( ccoords[first+11] < b ) ccoords[first+11] = b;
}

static inline void RL_PrepareDrawLights(void)
{
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glColorPointer(3, GL_FLOAT, 0, ccoords);
}

static inline void RL_PopulateVCoords(RetroWorldTileset_t *s, int x, int y, int first)
{
    int vfirst = first * 8;

    vcoords[vfirst+0] = x + 0;     vcoords[vfirst+1] = y + 0;
    vcoords[vfirst+2] = x + s->w;  vcoords[vfirst+3] = y + 0;
    vcoords[vfirst+4] = x + s->w;  vcoords[vfirst+5] = y + s->h;
    vcoords[vfirst+6] = x + 0;     vcoords[vfirst+7] = y + s->h;
}

static inline void RL_PrepareScreen(RetroWorldScreen_t *s)
{
    int x, y, no = 0;

    vcoords = realloc(vcoords, sizeof(float) * (8*s->w*s->h));
    ccoords = realloc(ccoords, sizeof(float) * (12*s->w*s->h));

    memset(ccoords, '\0', sizeof(float) * (12*s->w*s->h));

    for (y=0; y<s->h; y++)
    {
        for (x=0; x<s->w; x++)
        {
            RL_PopulateVCoords(s->ts, x * s->tw, y * s->th, no);
            no++;
        }
    }

}

static inline void RL_Trace(RetroWorldScreen_t *w, int x, int y, float r, float g, float b, float lr, float lg, float lb, int xd, int yd)
{
    float nr, ng, nb;

    if ( x < 0 || y < 0 || x >= w->w || y >= w->h )
        return;

    if ( r == 0 && g == 0 && b == 0 )
        return;

    if ( xd != 0 && yd != 0 )
    {
        nr = r - (lr*1.46);
        ng = g - (lg*1.46);
        nb = b - (lb*1.46);
    }
    else
    {
        nr = r - lr;
        ng = g - lg;
        nb = b - lb;
    }

    RL_SetTileColor(w, x, y, nr, ng, nb);

    if ( w->colmap[(y * w->w) + x] == RW_COL_SOLID )
        return;

    RL_Trace(w, x+xd, y+yd, nr, ng, nb, lr, lg, lb, xd, yd);
    if ( xd == 1 && yd == -1 )
    {
        RL_Trace(w, x+1, y, nr, ng, nb, lr, lg, lb, 1, 0);
        RL_Trace(w, x, y-1, nr, ng, nb, lr, lg, lb, 0, -1);
    }
    else if ( xd == 1 && yd == 1 )
    {
        RL_Trace(w, x+1, y, nr, ng, nb, lr, lg, lb, 1, 0);
        RL_Trace(w, x, y+1, nr, ng, nb, lr, lg, lb, 0, 1);
    }
    else if ( xd == -1 && yd == 1 )
    {
        RL_Trace(w, x-1, y, nr, ng, nb, lr, lg, lb, -1, 0);
        RL_Trace(w, x, y+1, nr, ng, nb, lr, lg, lb, 0, 1);
    }
    else if ( xd == -1 && yd == -1 )
    {
        RL_Trace(w, x-1, y, nr, ng, nb, lr, lg, lb, -1, 0);
        RL_Trace(w, x, y-1, nr, ng, nb, lr, lg, lb, 0, -1);
    }
}

static inline void RL_TraceEmitter(RetroSprite_t *s, RetroWorldScreen_t *w)
{
    int x, y;
    float loss_r, loss_g, loss_b;

    x = s->x + (s->spr->w / 2);
    y = s->y + (s->spr->h / 2);

    x /= w->tw;
    y /= w->th;

    loss_r = s->light.r / s->light.radius;
    loss_g = s->light.g / s->light.radius;
    loss_b = s->light.b / s->light.radius;    

    RL_SetTileColor(w, x, y, s->light.r, s->light.g, s->light.b);

    RL_Trace(w, x, y-1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, 0, -1);
    RL_Trace(w, x+1, y-1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, 1, -1);
    RL_Trace(w, x+1, y, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, 1, 0);
    RL_Trace(w, x+1, y+1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, 1, 1);
    RL_Trace(w, x, y+1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, 0, 1);
    RL_Trace(w, x-1, y+1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, -1, 1);
    RL_Trace(w, x-1, y, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, -1, 0);
    RL_Trace(w, x-1, y-1, s->light.r, s->light.g, s->light.b, loss_r, loss_g, loss_b, -1, -1);
}

static inline void RL_TraceEmitters(RetroAssets_t *a, RetroWorldScreen_t *w)
{
    RetroSprite_t *s;
    int i;
    for (i=0;i < a->s[a->current]->s_n; i++)
    {
        s = a->s[a->current]->s[i];

        if ( s->light.emits == 0 )
            continue;

        RL_TraceEmitter(s, w);
    }
}

void RL_DrawLights(RetroAssets_t *a, int colz)
{
    RetroWorldScreen_t *s = a->w[a->current]->w[colz];

    RL_PrepareScreen(s);
    RL_PrepareDrawLights();
    RL_TraceEmitters(a, s);
    glDrawArrays(GL_QUADS, 0, 4*s->w*s->h);

    glDisableClientState(GL_COLOR_ARRAY);
}
