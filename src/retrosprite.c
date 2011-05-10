#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include <GL/gl.h>

#include "retrosprite.h"
#include "retrocommon.h"

struct gfxtable {
    RetroSpriteGfx_t **g;
    int n;
};
static struct gfxtable gfxtable;
static RetroSpriteStats_t stats;


void RS_ResetStats(void)
{
    stats.spr_drawn = 0;
    stats.box_drawn = 0;
    stats.col_checks = 0;
    stats.col_found = 0;
}

void RS_PrintStats(void)
{
    printf("Retrosprite Statistics:\n");
    printf("     spr.drawn: %8d\n", stats.spr_drawn);
    printf("     box.drawn: %8d\n", stats.box_drawn);
    printf("    col.checks: %8d\n", stats.col_checks);
    printf("     col.found: %8d\n", stats.col_found);
    printf("\n");
}

static void RS_DrawBox(RetroSprite_t *s)
{
    float vcoords[] = {
        s->spr->anims[s->anim].box.x, s->spr->anims[s->anim].box.y,
        s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w, s->spr->anims[s->anim].box.y,
        s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w, s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h,
        s->spr->anims[s->anim].box.x, s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h,
    };
    glColor4f(1, 0, 0, 0.5);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glPushMatrix();
    glTranslatef(s->x, s->y, 0.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();

    stats.box_drawn++;
}

void RS_DrawBoxArray(RetroSprite_t *arr, uint16_t elements)
{
    int i;
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    for (i=0;i<elements;i++)
    {
        RS_DrawBox(&arr[i]);
    }
}

void RS_DrawSprite(RetroSprite_t *s)
{
//    printf("Texwidth: %d, framewidth: %d\n", s->spr->gfx->w, s->spr->w);

    float fw = (float)s->spr->w / (float)s->spr->gfx->w;
    float fh = (float)s->spr->h / (float)s->spr->gfx->h;

    int fpw = s->spr->gfx->w / s->spr->w;
    int fph = s->spr->gfx->h / s->spr->h;

    int fid = s->spr->anims[s->anim].indices[s->frame];

    float vcoords[] = {
        0, 0,
        s->spr->w, 0,
        s->spr->w, s->spr->h,
        0, s->spr->h,
    };

    float tcoords[8];

    tcoords[0] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[1] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[2] = fw  + ( fw * ( fid % fpw ) ); tcoords[3] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[4] = fw  + ( fw * ( fid % fpw ) ); tcoords[5] = fh  + ( fh * ( fid / fph ) );
    tcoords[6] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[7] = fh  + ( fh * ( fid / fph ) );


    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, s->spr->gfx->gl);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);

    glPushMatrix();
    glTranslatef(s->x + s->spr->anims[s->anim].offset.x, s->y + s->spr->anims[s->anim].offset.y, 0.0);
    if ( s->flags & RS_FLAGS_FLIP_V )
    {
        glTranslatef(s->spr->w, 0, 0);
        glRotatef(180, 0, 1, 0);
    }
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    
    glPopMatrix();

    stats.spr_drawn++;
}

void RS_DrawArray(RetroSprite_t *arr[], uint16_t elements)
{
    int i;
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    for (i=0;i<elements;i++)
    {
//        printf("element: %d\n", i);
        RS_DrawSprite(arr[i]);
    }
}

void RS_UpdateSprite(RetroSprite_t *s)
{
    s->frame++;
    if ( s->spr->anims[s->anim].flags == RS_ANIM_ONCE && s->frame >= s->spr->anims[s->anim].frames )
    {
        s->frame = 0;
        s->anim = s->spr->anims[s->anim].next_anim;
    }
    else if ( s->frame >= s->spr->anims[s->anim].frames )
        s->frame = 0;
}

void RS_SaveRetroSpriteGfx(char *filename, RetroSpriteGfx_t *g)
{
    int i;
    FILE *f = fopen(filename, "w");
    if ( f == NULL )
    {
        printf("Could not create or open file, for some reason.\n");
        return;
    }
    
    fwrite(&(g->anims_n), sizeof(uint16_t), 1, f);

    for (i=0;i<g->anims_n;i++)
        fwrite(&(g->anims[i]), offsetof(RetroSpriteGfxAnim_t, indices), 1, f);

    for (i=0;i<g->anims_n;i++)
        fwrite(g->anims[i].indices, sizeof(uint16_t), g->anims[i].frames, f);

    fclose(f);
    printf("%s written.\n", filename);
}

RetroSpriteGfx_t * RS_LoadRetroSpriteGfx(char *filename)
{
    size_t returned;
    int i;
    RetroSpriteGfx_t *g = calloc(sizeof(RetroSpriteGfx_t), 1);
    FILE *f = fopen(filename, "rb");
    if ( f == NULL )
    {
        printf("Could not open file\n");
        free(g);
        return NULL;
    }

    returned = fread(&(g->anims_n), sizeof(uint16_t), 1, f);
    g->anims = calloc(sizeof(RetroSpriteGfxAnim_t), g->anims_n);
    for (i=0;i<g->anims_n;i++)
    {
        returned = fread(&(g->anims[i]), offsetof(RetroSpriteGfxAnim_t, indices), 1, f);
/*        printf("Read %u bytes for animation %d\n", returned, i);
        printf("Animation no #%d\n"
               "  - frames: %d\n"
               "  - box: %d %d %d %d\n"
               "  - offset: %d %d\n"
               "  - flags: %d\n"
               "  - next_anim: %d\n", i, 
               g->anims[i].frames, g->anims[i].box.x, g->anims[i].box.y, g->anims[i].box.w, g->anims[i].box.h, g->anims[i].offset.x, g->anims[i].offset.y, g->anims[i].flags, g->anims[i].next_anim);*/
    }
    for (i=0;i<g->anims_n;i++)
    {
        g->anims[i].indices = calloc(sizeof(uint16_t), g->anims[i].frames);
        returned = fread(g->anims[i].indices, sizeof(uint16_t), g->anims[i].frames, f);
//        printf("Read %d / %d indices.\n", returned, g->anims[i].frames);
    }

    fclose(f);

    return g;
}

RetroSpriteGfx_t * RS_LoadRetroSpriteGfxwTex(char *filename, char *texfile, int w, int h)
{
    int n;
    RetroSpriteGfx_t *g = RS_LoadRetroSpriteGfx(filename);
    g->gfx = RC_GetGfx(texfile);
    return g;
}


void RS_UpdateArray(RetroSprite_t * arr[], int els)
{
    int i;
    for(i=0;i<els;i++) {
        RS_UpdateSprite(arr[i]);
    }
}

void RS_PerformLogicArray(RetroSprite_t * arr[], int els, void *data)
{
    int i;
    for (i=0;i<els;i++) {
        RS_PerformLogic(arr[i], data);
    }
}

void RS_PerformLogic(RetroSprite_t *s, void *data)
{
    if ( s->logic->l[s->anim] )
    {
        (s->logic->l[s->anim])(s, data);
    }
}

int RS_CheckCol(RetroSprite_t *s1, RetroSprite_t *s2)
{
    if ( RC_ColRect(
        s1->x + s1->spr->anims[s1->anim].box.x,
        s1->y + s1->spr->anims[s1->anim].box.y,
        s1->spr->anims[s1->anim].box.w,
        s1->spr->anims[s1->anim].box.h,
        s2->x + s2->spr->anims[s2->anim].box.x,
        s2->y + s2->spr->anims[s2->anim].box.y,
        s2->spr->anims[s2->anim].box.w,
        s2->spr->anims[s2->anim].box.h
        ) )
        return 1;
    return 0;
}

RetroSpriteGfx_t *RS_GetGfx(char *rsfile, char *gfxfile, int w, int h)
{
    int i;
    RetroSpriteGfx_t *g;

    for(i=0;i<gfxtable.n;i++)
    {
        if ( strcmp(rsfile, gfxtable.g[i]->filename) == 0 )
        {
            if ( strcmp(gfxfile, gfxtable.g[i]->gfx->filename) == 0 )
            {
                return gfxtable.g[i];
            }
        }
    }

    g = RS_LoadRetroSpriteGfx(rsfile);
    g->gfx = RC_GetGfx(gfxfile);
    g->w = w;
    g->h = h;

    gfxtable.n++;
    gfxtable.g = realloc(gfxtable.g, sizeof(void*)*gfxtable.n);
    gfxtable.g[gfxtable.n-1] = g;

    return gfxtable.g[gfxtable.n-1];
}


void RS_PushTable(RetroSpriteTable_t *t, RetroSprite_t *s)
{
    if ( !s )
        return;

    t->s_n++;

    t->s = realloc(t->s, t->s_n * sizeof(void*));
    t->s[t->s_n-1] = s;
}

RetroSpriteTable_t *RS_NewTable(void)
{
    RetroSpriteTable_t *t = calloc(sizeof(RetroSpriteTable_t), 1);

    return t;
}

RetroSprite_t *RS_NewSprite(int x, int y, int z, RetroSpriteGfx_t *g, RetroSpriteLogic_t *l, int anim)
{
    RetroSprite_t *s = calloc(sizeof(RetroSprite_t), 1);

    s->x = x;
    s->y = y;
    s->z = z;
    s->spr = g;
    s->logic = l;
    s->anim = anim;

    return s;

}
