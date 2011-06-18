#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "debug.h"

#include "retroworld.h"

static float *vcoords = NULL;
static float *tcoords = NULL;

static inline void RW_PrepareDrawTile(RetroWorldTileset_t *s)
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, s->gfx->gl);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
}

/*
    TODO: This function can be improved by precalculating most of these values.
*/
void RW_DrawTile(RetroWorldTileset_t *s, uint16_t id, int x, int y, int first)
{
    float fw = (float)s->w / (float)s->gfx->w;
    float fh = (float)s->h / (float)s->gfx->h;

    int fpw = s->gfx->w / s->w;
    int fph = s->gfx->h / s->h;

    int fid = 0;

    if ( id == 0 ) fid = 0;
    else if ( id <= fpw * fph ) fid = id - 1;
    else fid = s->anims[id - (fpw * fph) -1].indices[s->anims[id - (fpw * fph) -1].frame];

    first *= 8;

    vcoords[first+0] = x + 0;     vcoords[first+1] = y + 0;
    vcoords[first+2] = x + s->w;  vcoords[first+3] = y + 0;
    vcoords[first+4] = x + s->w;  vcoords[first+5] = y + s->h;
    vcoords[first+6] = x + 0;     vcoords[first+7] = y + s->h;

    tcoords[first+0] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[first+1] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[first+2] = fw  + ( fw * ( fid % fpw ) ); tcoords[first+3] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[first+4] = fw  + ( fw * ( fid % fpw ) ); tcoords[first+5] = fh  + ( fh * ( fid / fph ) );
    tcoords[first+6] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[first+7] = fh  + ( fh * ( fid / fph ) );
}

void RW_DrawTileAt(RetroWorldTileset_t *s, uint16_t id, int x, int y)
{
    vcoords = realloc(vcoords, sizeof(float) * 8);
    tcoords = realloc(tcoords, sizeof(float) * 8);

    RW_PrepareDrawTile(s);
    glColor4f(1, 1, 1, 1);

    glPushMatrix();
    RW_DrawTile(s, id, x, y, 0);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();
}

static void RW_UpdateTileAni(RetroWorldTileani_t *a)
{
    a->frame++;
    if ( a->frame >= a->frames )
    {
        a->frame = 0;
    }
}

void RW_UpdateTileset(RetroWorldTileset_t *ts)
{
    int i;
    for (i=0;i<ts->anims_n;i++)
    {
        RW_UpdateTileAni(&(ts->anims[i]));
    }
}

void RW_SaveRetroWorldTileset(char *filename, RetroWorldTileset_t *r)
{
    int i;
    FILE *f = fopen(filename, "w");
    if ( f == NULL )
    {
        retrodebug("Could not create or open file, for some reason.\n");
        return;
    }

    fwrite(&(r->anims_n), sizeof(uint16_t), 1, f);

    for (i=0;i<r->anims_n;i++)
    {
        fwrite(&(r->anims[i].frames), sizeof(uint16_t), 1, f);
        fwrite(r->anims[i].indices, sizeof(uint16_t), r->anims[i].frames, f);
    }

    fclose(f);
    retrodebug("%s written.\n", filename);
}

RetroWorldTileset_t * RW_LoadRetroWorldTileset(char *filename)
{
    size_t returned;
    int i, y;
    RetroWorldTileset_t *g = calloc(sizeof(RetroWorldTileset_t), 1);
    FILE *f = fopen(filename, "rb");
    if ( f == NULL )
    {
        retrodebug("Could not open file (%s)\n", filename);
        free(g);
        return NULL;
    }

    returned = fread(&(g->anims_n), sizeof(uint16_t), 1, f);
    g->anims = calloc(sizeof(RetroWorldTileani_t), g->anims_n);
    for (i=0;i<g->anims_n;i++)
    {
        returned = fread(&(g->anims[i].frames), sizeof(uint16_t), 1, f);
        g->anims[i].indices = calloc(sizeof(uint16_t), g->anims[i].frames);
        returned = fread(g->anims[i].indices, sizeof(uint16_t), g->anims[i].frames, f);
    }

    fclose(f);

    retrodebug("Loaded '%s'\n", filename);

    return g;
}

RetroWorldTileset_t * RW_LoadRetroWorldTilesetwTex(char *filename, char *texfile, int w, int h)
{
    int n;
    RetroWorldTileset_t *g = RW_LoadRetroWorldTileset(filename);
    g->gfx = RC_GetGfx(texfile);
    g->w = w;
    g->h = h;

    retrodebug("RW_LoadRetroWorldTilesetwTex: Loaded '%s' and '%s' - %d x %d\n", filename, texfile, w, h);

    return g;
}

void RW_DrawScreen(RetroWorldScreen_t *s, int grid)
{
    int x, y, no = 0;
    glColor4f(1, 1, 1, 1);

    vcoords = realloc(vcoords, sizeof(float) * (8*s->w*s->h));
    tcoords = realloc(tcoords, sizeof(float) * (8*s->w*s->h));
    RW_PrepareDrawTile(s->ts);

    glPushMatrix();
    for (y=0; y<s->h; y++)
    {
        for (x=0; x<s->w; x++)
        {
            RW_DrawTile(s->ts, s->map[(y*s->w)+x], x * s->tw, y * s->th, no);
            no++;
        }
    }
    glDrawArrays(GL_QUADS, 0, 4*s->w*s->h);
    glPopMatrix();
}

void RW_DrawScreenColmap(RetroWorldScreen_t *s, RetroWorldTileset_t *ts)
{
    int x, y, no = 0;
    glColor4f(1, 1, 1, 1);

    vcoords = realloc(vcoords, sizeof(float) * (8*s->w*s->h));
    tcoords = realloc(tcoords, sizeof(float) * (8*s->w*s->h));

    glPushMatrix();
    for (y=0; y<s->h; y++)
    {
        for (x=0; x<s->w; x++)
        {
            RW_DrawTile(ts, s->colmap[(y*s->w)+x], x * s->tw, y * s->th, no);
            no++;
        }
    }
    glDrawArrays(GL_QUADS, 0, 4*s->w*s->h);
    glPopMatrix();
}


void RW_SaveRetroWorldScreen(char *filename, RetroWorldScreen_t *s)
{
    int i;
    FILE *f = fopen(filename, "w");
    if ( f == NULL )
    {
        retrodebug("Could not create or open file, for some reason.\n");
        return;
    }

    fwrite(&(s->w), sizeof(uint16_t), 1, f);
    fwrite(&(s->h), sizeof(uint16_t), 1, f);

    fwrite(s->map, sizeof(uint16_t), s->w * s->h, f);
    fwrite(s->colmap, sizeof(uint8_t), s->w * s->h, f);

    fclose(f);
    retrodebug("%s written.\n", filename);
}

RetroWorldScreen_t * RW_LoadRetroWorldScreen(char *filename)
{
    size_t returned;
    int i, y;
    RetroWorldScreen_t *s = calloc(sizeof(RetroWorldScreen_t), 1);
    FILE *f = fopen(filename, "rb");
    if ( f == NULL )
    {
        retrodebug("Could not open file (%s)\n", filename);
        free(s);
        return NULL;
    }

    returned = fread(&(s->w), sizeof(uint16_t), 1, f);
    returned = fread(&(s->h), sizeof(uint16_t), 1, f);

    s->map = calloc(sizeof(uint16_t), s->w * s->h);
    s->colmap = calloc(sizeof(uint8_t), s->w * s->h);

    returned = fread(s->map, sizeof(uint16_t), s->w * s->h, f);

    returned = fread(s->colmap, sizeof(uint8_t), s->w * s->h, f);

    fclose(f);

//    retrodebug("RW_LoadRetroWorldScreen: Loaded '%s' (%d, %d)\n", filename, s->w, s->h);

    return s;
}

RetroWorldScreen_t * RW_LoadRetroWorldScreenwTS(char *filename, RetroWorldTileset_t *ts)
{
    RetroWorldScreen_t *sc = RW_LoadRetroWorldScreen(filename);
    sc->ts = ts;
    sc->tw = sc->ts->w;
    sc->th = sc->ts->h;

    return sc;
}

void RW_PushTable(RetroWorldScreenTable_t *t, RetroWorldScreen_t *s)
{
    if ( !s )
        return;

    t->w_n++;

    t->w = realloc(t->w, t->w_n * sizeof(void*));
    t->w[t->w_n-1] = s;
}

void RW_PushConnToTable(RetroWorldScreenTable_t *t, uint8_t fx, uint8_t fy, uint8_t fw, uint8_t fh,
                        uint32_t toscreen, uint8_t tx, uint8_t ty, uint8_t tw, uint8_t th)
{
    t->w_c++;
    retrodebug("Allocating!\n");
    t->c = realloc(t->c, t->w_c * sizeof(RetroScreenConn_t));

    retrodebug("Allocated!\n");

    t->c[t->w_c-1].fx = fx;
    t->c[t->w_c-1].fy = fy;
    t->c[t->w_c-1].fw = fw;
    t->c[t->w_c-1].fh = fh;
    t->c[t->w_c-1].toscreen = toscreen;
    t->c[t->w_c-1].tx = tx;
    t->c[t->w_c-1].ty = ty;
    t->c[t->w_c-1].tw = tw;
    t->c[t->w_c-1].th = th;
    retrodebug("Populated!\n");
}

RetroWorldScreenTable_t *RW_NewTable(void)
{
    RetroWorldScreenTable_t *t = calloc(sizeof(RetroWorldScreenTable_t), 1);

    return t;
}

