#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "retroworld.h"

void RW_DrawTile(RetroWorldTileset_t *s, uint16_t id)
{
    float fw = (float)s->w / (float)s->tw;
    float fh = (float)s->h / (float)s->th;
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    int fpw = s->tw / s->w;
    int fph = s->th / s->h;

    int fid = 0;

    if ( id == 0 ) return;
    else if ( id <= fpw * fph ) fid = id - 1;
    else fid = s->anims[id - (fpw * fph) -1].indices[s->anims[id - (fpw * fph) -1].frame];


    float vcoords[] = {
        0, 0,
        s->w, 0,
        s->w, s->h,
        0, s->h,
    };

    float tcoords[8];

    tcoords[0] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[1] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[2] = fw  + ( fw * ( fid % fpw ) ); tcoords[3] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[4] = fw  + ( fw * ( fid % fpw ) ); tcoords[5] = fh  + ( fh * ( fid / fph ) );
    tcoords[6] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[7] = fh  + ( fh * ( fid / fph ) );


    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, s->gl_texture);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);

    glPushMatrix();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
        printf("Could not create or open file, for some reason.\n");
        return;
    }

    fwrite(&(r->anims_n), sizeof(uint16_t), 1, f);

    for (i=0;i<r->anims_n;i++)
    {
        fwrite(&(r->anims[i].frames), sizeof(uint16_t), 1, f);
        fwrite(r->anims[i].indices, sizeof(uint16_t), r->anims[i].frames, f);
    }

    fclose(f);
    printf("%s written.\n", filename);
}

RetroWorldTileset_t * RW_LoadRetroWorldTileset(char *filename)
{
    size_t returned;
    int i, y;
    RetroWorldTileset_t *g = calloc(sizeof(RetroWorldTileset_t), 1);
    FILE *f = fopen(filename, "r");
    if ( f == NULL )
    {
        printf("Could not open file\n");
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

    return g;
}

void RW_DrawScreen(RetroWorldScreen_t *s, int grid)
{
    int x, y;
    glColor4f(1, 1, 1, 1);

    float square[] = {
        0.0, 0.0,
        s->tw, 0.0,
        s->tw, s->th,
        0, s->th
    };

    glPushMatrix();
    for (y=0; y<s->h; y++)
    {
        for (x=0; x<s->w; x++)
        {
            RW_DrawTile(s->ts, s->map[(y*s->w)+x]);
            if ( grid == 1 )
            {
                glDisable(GL_TEXTURE_2D);
                glVertexPointer(2,GL_FLOAT,0,square);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDrawArrays(GL_LINE_LOOP,0,4);
            }
            glTranslatef(s->tw, 0, 0);
        }
        glTranslatef((s->tw * s->w) * -1, s->th, 0);
    }
    glPopMatrix();
}

void RW_DrawScreenColmap(RetroWorldScreen_t *s, RetroWorldTileset_t *ts)
{
    int x, y;

    glPushMatrix();
    for (y=0; y<s->h; y++)
    {
        for (x=0; x<s->w; x++)
        {
            RW_DrawTile(ts, s->colmap[(y*s->w)+x]);
            glTranslatef(s->tw, 0, 0);
        }
        glTranslatef((s->tw * s->w) * -1, s->th, 0);
    }
    glPopMatrix();
}


void RW_SaveRetroWorldScreen(char *filename, RetroWorldScreen_t *s)
{
    int i;
    FILE *f = fopen(filename, "w");
    if ( f == NULL )
    {
        printf("Could not create or open file, for some reason.\n");
        return;
    }

    fwrite(&(s->w), sizeof(uint16_t), 1, f);
    fwrite(&(s->h), sizeof(uint16_t), 1, f);

    fwrite(s->map, sizeof(uint16_t), s->w * s->h, f);
    fwrite(s->colmap, sizeof(uint8_t), s->w * s->h, f);

    fclose(f);
    printf("%s written.\n", filename);
}

RetroWorldScreen_t * RW_LoadRetroWorldScreen(char *filename)
{
    size_t returned;
    int i, y;
    RetroWorldScreen_t *s = calloc(sizeof(RetroWorldScreen_t), 1);
    FILE *f = fopen(filename, "r");
    if ( f == NULL )
    {
        printf("Could not open file\n");
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

    return s;
}

