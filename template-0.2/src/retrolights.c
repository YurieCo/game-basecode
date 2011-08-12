#include <string.h>
#include <SDL_opengl.h>

#include "retrolights.h"

#define AVG2(n1, n2) ((n1+n2)/2)
#define AVG4(n1, n2, n3, n4) ((n1+n2+n3+n4)/4)
#define LOSS_M(a, b, c)  ( ((float)a / (float)b) * c )
#define MAX(a, b) (a > b ? a : b)

struct color {
	float r, g, b;
};

static float *vcoords = NULL;
static struct color *ccoords = NULL;
static struct color *ccoords_t = NULL;

static inline void RL_SetTileColor(RetroWorldScreen_t *w, int x, int y, float r, float g, float b)
{
    int first = (y * w->w) + x;

    if ( ccoords_t[first+0].r < r ) ccoords_t[first+0].r = r;
    if ( ccoords_t[first+0].g < g ) ccoords_t[first+0].g = g;
    if ( ccoords_t[first+0].b < b ) ccoords_t[first+0].b = b;
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
    ccoords = realloc(ccoords, sizeof(struct color) * (4*s->w*s->h));
	ccoords_t = realloc(ccoords_t, sizeof(struct color) * (s->w*s->h));

    memset(ccoords, '\0', sizeof(struct color) * (4*s->w*s->h));
	memset(ccoords_t, '\0', sizeof(struct color) * (s->w*s->h));

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

    if ( r <= 0 && g <= 0 && b <= 0 )
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
    int x, y, lx, ly, rlx, rly;
    float loss_r, loss_g, loss_b, mlr, mlg, mlb;

    lx = x = s->x + (s->spr->w / 2);
    ly = y = s->y + (s->spr->h / 2);

    x /= w->tw;
    y /= w->th;

    lx %= w->tw;
    ly %= w->th;
	lx -= w->tw/2;
	ly -= w->th/2;
	rlx = lx * -1;
	rly = ly * -1;
	

    mlr = loss_r = s->light.r / s->light.radius;
    mlg = loss_g = s->light.g / s->light.radius;
    mlb = loss_b = s->light.b / s->light.radius;

    mlr /= w->tw;
    mlg /= w->tw;
    mlb /= w->tw;

    RL_SetTileColor(w, x, y, s->light.r, s->light.g, s->light.b);


    RL_Trace(w, x,   y-1, s->light.r - LOSS_M(ly, w->th, loss_r),
						  s->light.g - LOSS_M(ly, w->th, loss_g),
						  s->light.b - LOSS_M(ly, w->th, loss_b), loss_r, loss_g, loss_b, 0, -1);
    RL_Trace(w, x+1, y-1, s->light.r - LOSS_M(MAX(rlx, ly), w->tw, loss_r)*0.73, 
						  s->light.g - LOSS_M(MAX(rlx, ly), w->tw, loss_g)*0.73, 
						  s->light.b - LOSS_M(MAX(rlx, ly), w->tw, loss_b)*0.73, loss_r, loss_g, loss_b, 1, -1);
    RL_Trace(w, x+1, y,   s->light.r - LOSS_M(rlx, w->tw, loss_r),
						  s->light.g - LOSS_M(rlx, w->tw, loss_g),
						  s->light.b - LOSS_M(rlx, w->tw, loss_b), loss_r, loss_g, loss_b, 1, 0);
    RL_Trace(w, x+1, y+1, s->light.r - LOSS_M(MAX(rlx, rly), w->tw, loss_r)*0.73, 
						  s->light.g - LOSS_M(MAX(rlx, rly), w->tw, loss_g)*0.73, 
						  s->light.b - LOSS_M(MAX(rlx, rly), w->tw, loss_b)*0.73, loss_r, loss_g, loss_b, 1, 1);
    RL_Trace(w, x,   y+1, s->light.r - LOSS_M(rly, w->th, loss_r),
						  s->light.g - LOSS_M(rly, w->th, loss_g), 
						  s->light.b - LOSS_M(rly, w->th, loss_b), loss_r, loss_g, loss_b, 0, 1);
    RL_Trace(w, x-1, y+1, s->light.r - LOSS_M(MAX(lx, rly), w->tw, loss_r)*0.73, 
						  s->light.g - LOSS_M(MAX(lx, rly), w->tw, loss_g)*0.73, 
						  s->light.b - LOSS_M(MAX(lx, rly), w->tw, loss_b)*0.73, loss_r, loss_g, loss_b, -1, 1);
    RL_Trace(w, x-1, y,   s->light.r - LOSS_M(lx, w->tw, loss_r), 
						  s->light.g - LOSS_M(lx, w->tw, loss_g), 
						  s->light.b - LOSS_M(lx, w->tw, loss_b), loss_r, loss_g, loss_b, -1, 0);
    RL_Trace(w, x-1, y-1, s->light.r - LOSS_M(MAX(lx, ly), w->tw, loss_r)*0.73, 
						  s->light.g - LOSS_M(MAX(lx, ly), w->tw, loss_g)*0.73, 
						  s->light.b - LOSS_M(MAX(lx, ly), w->tw, loss_b)*0.73, loss_r, loss_g, loss_b, -1, -1);
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

static inline void RL_InterPolate(RetroWorldScreen_t *w)
{
	int x, y;
	int first, first_t;
	for (x = 1; x < w->w-1; x++)
	{
		for (y = 1; y < w->h-1; y++)
		{
			first_t = ((y * w->w) + x);
			first = first_t * 4;

		    ccoords[first+0].r = AVG4(ccoords_t[first_t -1].r,
									  ccoords_t[first_t].r,
									  ccoords_t[first_t -1 - w->w].r, 
									  ccoords_t[first_t -w->w].r);
            ccoords[first+0].g = AVG4(ccoords_t[first_t -1].g,
                                      ccoords_t[first_t].g,
                                      ccoords_t[first_t -1 - w->w].g, 
                                      ccoords_t[first_t -w->w].g);
            ccoords[first+0].b = AVG4(ccoords_t[first_t -1].b,
                                      ccoords_t[first_t].b,
                                      ccoords_t[first_t -1 - w->w].b,
                                      ccoords_t[first_t -w->w].b);


            ccoords[first+1].r = AVG4(ccoords_t[first_t].r,
                                      ccoords_t[first_t+1].r,
                                      ccoords_t[first_t-w->w].r,
                                      ccoords_t[first_t+1-w->w].r);
            ccoords[first+1].g = AVG4(ccoords_t[first_t].g,
                                      ccoords_t[first_t+1].g,
                                      ccoords_t[first_t-w->w].g,
                                      ccoords_t[first_t+1-w->w].g);
            ccoords[first+1].b = AVG4(ccoords_t[first_t].b,
                                      ccoords_t[first_t+1].b,
                                      ccoords_t[first_t-w->w].b,
                                      ccoords_t[first_t+1-w->w].b);

            ccoords[first+2].r = AVG4(ccoords_t[first_t].r,
                                      ccoords_t[first_t+1].r,
                                      ccoords_t[first_t+w->w].r,
                                      ccoords_t[first_t+1+w->w].r);
            ccoords[first+2].g = AVG4(ccoords_t[first_t].g,
                                      ccoords_t[first_t+1].g,
                                      ccoords_t[first_t+w->w].g,
                                      ccoords_t[first_t+1+w->w].g);
            ccoords[first+2].b = AVG4(ccoords_t[first_t].b,
                                      ccoords_t[first_t+1].b,
                                      ccoords_t[first_t+w->w].b,
                                      ccoords_t[first_t+1+w->w].b);


            ccoords[first+3].r = AVG4(ccoords_t[first_t].r,
                                      ccoords_t[first_t-1].r,
                                      ccoords_t[first_t+w->w].r,
                                      ccoords_t[first_t-1+w->w].r);
            ccoords[first+3].g = AVG4(ccoords_t[first_t].g,
                                      ccoords_t[first_t-1].g,
                                      ccoords_t[first_t+w->w].g,
                                      ccoords_t[first_t-1+w->w].g);
            ccoords[first+3].b = AVG4(ccoords_t[first_t].b,
                                      ccoords_t[first_t-1].b,
                                      ccoords_t[first_t+w->w].b,
                                      ccoords_t[first_t-1+w->w].b);
		}
	}
}

void RL_DrawLights(RetroAssets_t *a, int colz)
{
    RetroWorldScreen_t *s = a->w[a->current]->w[colz];

    RL_PrepareScreen(s);
    RL_PrepareDrawLights();
    RL_TraceEmitters(a, s);
	RL_InterPolate(s);
    glDrawArrays(GL_QUADS, 0, 4*s->w*s->h);

    glDisableClientState(GL_COLOR_ARRAY);
}
