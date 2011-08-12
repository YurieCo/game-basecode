#ifndef __RETROASSETS_H__
#define __RETROASSETS_H__

#include <stdint.h>

#include "retroworld.h"
#include "retrosprite.h"
#include "retrocommon.h"

#define RA_SLOPEH_NO    0
#define RA_SLOPEH_N     1
#define RA_SLOPEH_E     2
#define RA_SLOPEH_S     3
#define RA_SLOPEH_W     4

typedef struct retrocamera_t {
    int x, y; // Position
    uint32_t w, h; // Dimensions

    int min_l, min_r, min_t, min_b;

    int spr_x, spr_y, spr_w, spr_h;
}RetroCamera_t;

typedef struct retroassets_t {    
    RetroSpriteTable_t **s;
    RetroWorldScreenTable_t **w;
    RetroCamera_t c;

    // Table count.
    uint32_t a_n;

    // The index of the tables we currently want to focus on.
    uint32_t current;
    RetroSpriteLogic_t **l;
}RetroAssets_t;

RetroAssets_t * RA_LoadFile(char *filename, RetroSpriteLogic_t *log[]);
int RA_SnapToLadder(RetroSprite_t *s, RetroAssets_t *a);
int RA_MoveCheckWorldCol(RetroSprite_t *s, RetroAssets_t *a, int x, int y, int sw, int laddercol, int sh);
RetroCamera_t RA_Camera(uint32_t w, uint32_t h, int min_l, int min_r, int min_t, int min_b,
                        int spr_x, int spr_y, int spr_w, int spr_h);


void RA_CameraAdjust(RetroAssets_t *a, RetroSprite_t *s, RetroCamera_t *c);

#endif /* __RETROASSETS_H__ */
