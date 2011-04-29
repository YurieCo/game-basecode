#ifndef __RETROWORLD_H__
#define __RETROWORLD_H__

#include <stdint.h>

typedef struct retroworldtileani_t {
    uint16_t *indices;
    uint16_t frames;
    uint16_t frame;
    uint32_t attributes;
    uint16_t triggerid;
}RetroWorldTileani_t;

#define RW_TILEATTRIBUTE_SOLID          1
#define RW_TILEATTRIBUTE_TOUCHTRIGGER   2

typedef struct retroworldtileset_t {
    uint32_t gl_texture;
    char *pixels;
    uint32_t tw, th;
    uint16_t w, h;
    uint16_t anims_n;
    RetroWorldTileani_t *anims;
}RetroWorldTileset_t;

#define RW_TRIGGER_ONCE 1

typedef struct retroworldtrigger_t {
    uint32_t flags;
    uint32_t x, y, w, h;
    uint16_t trigger;
}RetroWorldTrigger_t;

#define RW_COL_NONE     0
#define RW_COL_SOLID    1
#define RW_COL_SLOPE_SW 2
#define RW_COL_SLOPE_SE 3
#define RW_COL_SLOPE_NE 4
#define RW_COL_SLOPE_NW 5
#define RW_COL_SOLID_N  6
#define RW_COL_SOLID_S  7
#define RW_COL_SOLID_W  8
#define RW_COL_SOLID_E  9

typedef struct retroworldscreen_t {
    uint16_t w,h;
    RetroWorldTileset_t *ts;
    uint8_t tw,th;
    uint16_t *map;
    uint8_t *colmap;
}RetroWorldScreen_t;

void RW_DrawTile(RetroWorldTileset_t *s, uint16_t id);

RetroWorldTileset_t * RW_LoadRetroWorldTileset(char *filename);
void RW_SaveRetroWorldTileset(char *filename, RetroWorldTileset_t *r);

void RW_UpdateTileset(RetroWorldTileset_t *ts);

void RW_DrawScreen(RetroWorldScreen_t *s, int grid);
void RW_DrawScreenColmap(RetroWorldScreen_t *s, RetroWorldTileset_t *ts);

RetroWorldScreen_t * RW_LoadRetroWorldScreen(char *filename);
void RW_SaveRetroWorldScreen(char *filename, RetroWorldScreen_t *s);


#endif /* __RETROWORLD_H__ */
