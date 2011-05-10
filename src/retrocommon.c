#include <stdlib.h>
#include <GL/gl.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retrocommon.h"

struct gfxtable {
    RetroGfx_t **g;
    int n;
};
struct gfxtable gfxtable;

RetroGfx_t *RC_GetGfx(char *filename)
{
    int i, tw, th, n;
    char *pixels;

    for(i=0;i<gfxtable.n;i++)
    {
        if ( strcmp(filename, gfxtable.g[i]->filename) == 0 )
            return gfxtable.g[i];
    }

    pixels = stbi_load(filename, &tw, &th, &n, 0);

    if ( !pixels )
        return NULL;


    RetroGfx_t *g = calloc(sizeof(RetroGfx_t), 1);

    g->w = tw;
    g->h = th;
    sprintf(g->filename, "%s", filename);

    glGenTextures(1, &(g->gl));
    glBindTexture(GL_TEXTURE_2D, g->gl);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, g->w, g->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);


    gfxtable.n++;
    gfxtable.g = realloc(gfxtable.g, sizeof(void*)*gfxtable.n);
    gfxtable.g[gfxtable.n-1] = g;

    stbi_image_free(pixels);

    return gfxtable.g[gfxtable.n-1];
}

int RC_ColRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    if ( x1 >= x2 + w2 )
        return 0;
    if ( y1 >= y2 + h2 )
        return 0;
    if ( x1 + w1 <= x2 )
        return 0;
    if ( y1 + h1 <= y2 )
        return 0;

    return 1;
}

