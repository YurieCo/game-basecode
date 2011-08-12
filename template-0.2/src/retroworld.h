#ifndef __RETROWORLD_H__
#define __RETROWORLD_H__

#include <stdint.h>

#include "retrocommon.h"

#define RW_SCREEN_LEFT       1
#define RW_SCREEN_RIGHT      2
#define RW_SCREEN_TOP        3
#define RW_SCREEN_BOTTOM     4

typedef struct retroscreenconn_t {
    uint8_t fx;
    uint8_t fy;
    uint8_t fw;
    uint8_t fh;
    uint32_t toscreen;
    uint8_t tx;
    uint8_t ty;
    uint8_t tw;
    uint8_t th;
}RetroScreenConn_t;


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
    RetroGfx_t *gfx;
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
#define RW_COL_SLOPE_SW 2   /*  .\  */
#define RW_COL_SLOPE_SE 3   /*  /.  */
#define RW_COL_SLOPE_NE 4   /*  \.  */
#define RW_COL_SLOPE_NW 5   /*  ./  */
#define RW_COL_SOLID_N  6
#define RW_COL_SOLID_S  7
#define RW_COL_SOLID_W  8
#define RW_COL_SOLID_E  9
#define RW_COL_LADDER   10

typedef struct retroworldscreen_t {
    uint16_t w,h;
    RetroWorldTileset_t *ts;
    uint8_t tw,th;
    uint16_t *map;
    uint8_t *colmap;
}RetroWorldScreen_t;

typedef struct retroworldscreentable_t {
    RetroWorldScreen_t **w;
    RetroScreenConn_t *c;
    
    uint32_t w_n;
    uint32_t w_c;
}RetroWorldScreenTable_t;

//void RW_DrawTile(RetroWorldTileset_t *s, uint16_t id);
void RW_DrawTile(RetroWorldTileset_t *s, uint16_t id, int x, int y, int first);
void RW_DrawTileAt(RetroWorldTileset_t *s, uint16_t id, int x, int y);

RetroWorldTileset_t * RW_LoadRetroWorldTileset(char *filename);
RetroWorldTileset_t * RW_LoadRetroWorldTilesetwTex(char *filename, char *texfile, int w, int h);
void RW_SaveRetroWorldTileset(char *filename, RetroWorldTileset_t *r);

void RW_UpdateTileset(RetroWorldTileset_t *ts);

void RW_DrawScreen(RetroWorldScreen_t *s, int grid);
void RW_DrawScreenColmap(RetroWorldScreen_t *s, RetroWorldTileset_t *ts);

RetroWorldScreen_t * RW_LoadRetroWorldScreen(char *filename);
RetroWorldScreen_t * RW_LoadRetroWorldScreenwTS(char *filename, RetroWorldTileset_t *ts);

void RW_SaveRetroWorldScreen(char *filename, RetroWorldScreen_t *s);

void RW_PushTable(RetroWorldScreenTable_t *t, RetroWorldScreen_t *s);
void RW_PushConnToTable(RetroWorldScreenTable_t *t, uint8_t fx, uint8_t fy, uint8_t fw, uint8_t fh, 
                        uint32_t toscreen, uint8_t tx, uint8_t ty, uint8_t tw, uint8_t th);

RetroWorldScreenTable_t *RW_NewTable(void);

#define RW_PrintRoom(s) do { \
        int i; \
        for (i=0;i<s->w*s->h;i++) { \
            printf(" %02x", s->map[i]); \
            if ( i % s->w == s->w-1 ) printf("\n");\
        }\
    } while (0);


#endif /* __RETROWORLD_H__ */
