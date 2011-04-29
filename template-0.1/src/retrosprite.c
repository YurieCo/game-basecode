#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "retrosprite.h"

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

static void RS_DrawSprite(RetroSprite_t *s)
{
    float fw = (float)s->spr->w / (float)s->spr->tw;
    float fh = (float)s->spr->h / (float)s->spr->th;

    int fpw = s->spr->tw / s->spr->w;
    int fph = s->spr->th / s->spr->h;


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
    glBindTexture(GL_TEXTURE_2D, s->spr->gl_texture);
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

void RS_DrawArray(RetroSprite_t *arr, uint16_t elements)
{
    int i;
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    for (i=0;i<elements;i++)
    {
        RS_DrawSprite(&arr[i]);
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
    FILE *f = fopen(filename, "r");
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

