#ifndef __RETROCOMMON_H__
#define __RETROCOMMON_H__

#include <stdint.h>

#define MAX_FILENAME_LENGTH 128

/*
    Retrocommon contains code reused across retroXX.
*/

/* TODO:

    Basic colission detection functions.

        * Circular colission detection.
        * Per pixel colission detection ? Perhaps keep that in RetroSprite.

    Image loading, basic reuse of loaded images.
*/

typedef struct retrogfx_t {
    char filename[MAX_FILENAME_LENGTH];
    uint32_t w, h;    
    uint32_t gl;
}RetroGfx_t;

RetroGfx_t *RC_GetGfx(char *filename);

int RC_ColRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void RC_DrawTexture(RetroGfx_t *t, int x, int y, int w, int h, int r, int g, int b, int a);

#endif /* __RETROCOMMON_H__ */
