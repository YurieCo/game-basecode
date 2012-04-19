#include <stdlib.h>
#include <GL/gl.h>
#include <AL/al.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retrocommon.h"
#include "retrosound.h"

struct gfxtable {
    RetroGfx_t **g;
    int n;
};
struct sndtable {
	RetroSnd_t **s;
	int n;
};

struct gfxtable gfxtable;
struct sndtable sndtable;

RetroSnd_t *RC_GetSnd(char *filename)
{
	ALenum format = NULL;
	int i;
	RetroSnd_t *s;

	RSO_Init();

	for(i=0;i<sndtable.n;i++)
    {
        if ( strcmp(filename, sndtable.s[i]->filename) == 0 )
            return sndtable.s[i];
    }
    printf("WE ARE LOADING THE SOUND FROM FILE!\n");

	s = calloc(sizeof(RetroSnd_t), 1);

	if ( SDL_LoadWAV(filename, &(s->spec), &(s->buf), &(s->length)) == NULL )
	{
		fprintf(stderr, "Could not open %s: %s\n", filename, SDL_GetError());
		return NULL;
	}
	sprintf(s->filename, "%s", filename);

	alGenBuffers(1, &(s->buffer));
	switch (s->spec.format) {
    	case AUDIO_U8:
      	case AUDIO_S8:
        	format = (s->spec.channels==1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
        	break;
      	case AUDIO_U16LSB:
      	case AUDIO_S16LSB:
        	format = (s->spec.channels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        	break;
    }

	alBufferData(s->buffer, format, s->buf, s->length, s->spec.freq);

	sndtable.n++;
	sndtable.s = realloc(sndtable.s, sizeof(void*)*sndtable.n);
	sndtable.s[sndtable.n-1] = s;

	return sndtable.s[sndtable.n-1];
}

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

void RC_DrawTexture(RetroGfx_t *t, int x, int y, int w, int h, int r, int g, int b, int a)
{
    float vcoords[] = {
        x, y,
        x+w, y,
        x+w, y+h,
        x, y+h
    };
    float tcoords[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4d(r, g, b, a);
    glBindTexture(GL_TEXTURE_2D, t->gl);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
