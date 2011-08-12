/*
 *  retrosprite.[ch]
 *
 *  Copyright 2011 Trygve Vea <trygve.vea@gmail.com>
 *
 *  
 */
#ifndef __RETROSPRITE_H__
#define __RETROSPRITE_H__

#include <stdlib.h>
#include <stdint.h>

#include "retrocommon.h"

typedef struct retrospritestats_t {
    uint32_t spr_drawn;
    uint32_t box_drawn;
    uint32_t col_checks;
    uint32_t col_found;
}RetroSpriteStats_t;

typedef struct retrospritebox_t {
    uint16_t x, y, w, h;
}__attribute__((__packed__)) RetroSpriteBox_t;

typedef struct retrospriteoffset_t {
    int16_t x, y;
}__attribute__((__packed__)) RetroSpriteOffset_t;

typedef struct retrospritegfxanim_t {
    uint16_t frames;
    RetroSpriteBox_t box;
    RetroSpriteOffset_t offset;
    uint32_t flags;
    uint16_t next_anim;
    uint16_t *indices;
}__attribute__((__packed__)) RetroSpriteGfxAnim_t;

typedef struct retrospritegfx_t {
    RetroGfx_t *gfx;
    char filename[MAX_FILENAME_LENGTH];

    uint16_t w, h;   // Frame width / height
//    uint32_t tw, th; // Texture width / height

    RetroSpriteGfxAnim_t *anims;
    uint16_t anims_n;
}__attribute__((__packed__)) RetroSpriteGfx_t;

enum {
    RS_LOGIC_DEFAULT,
    RS_LOGIC_ATTACK,
    RS_LOGIC_DIE,
};

#define RS_ANIM_LOOP    0
#define RS_ANIM_ONCE    1
#define RS_ANIM_MANUAL  2

#define RS_FLAGS_FLIP_V 1
#define RS_FLAGS_FLIP_H 2

typedef struct retrospritelogic_t {
    void (*l[256])();
}RetroSpriteLogic_t;

typedef struct retrolightinfo_t {
    int emits; // Bool, 0 means it doesn't emit any light.
    float radius; // 
    float r, g, b; // The color of the light.
}RetroLightInfo_t;

typedef struct retrosprite_t {
    int x, y, z; // Position

    RetroSpriteGfx_t *spr; // Reference to a RetroSpriteGfx_t
    uint32_t flags;
    uint16_t anim;
    uint16_t frame;

    int32_t registers[16];

    RetroSpriteLogic_t *logic; // Reference to a RetroSpriteLogic_t
    int kill;
    RetroLightInfo_t light;
}RetroSprite_t;

#define RS_SORTAXIS_N 0
#define RS_SORTAXIS_X 1
#define RS_SORTAXIS_Y 2
#define RS_SORTAXIS_Z 3

typedef struct retrospritetable_t {
    RetroSprite_t **s;

    uint32_t s_n;

    uint8_t sortaxis;
}RetroSpriteTable_t;

void RS_PopTable(RetroSpriteTable_t *t, RetroSprite_t *s);
void RS_PushTable(RetroSpriteTable_t *t, RetroSprite_t *s);
RetroSpriteTable_t *RS_NewTable(void);


void RS_DrawSprite(RetroSprite_t *s);
void RS_DrawArray(RetroSprite_t *arr[], uint16_t elements);
void RS_DrawBoxArray(RetroSprite_t *arr, uint16_t elements);
void RS_UpdateSprite(RetroSprite_t *s);

void RS_SaveRetroSpriteGfx(char *filename, RetroSpriteGfx_t *g);
RetroSpriteGfx_t * RS_LoadRetroSpriteGfx(char *filename);
RetroSpriteGfx_t * RS_LoadRetroSpriteGfxwTex(char *filename, char *texfile, int w, int h);

void RS_UpdateArray(RetroSprite_t * arr[], int els);

void RS_PerformLogic(RetroSprite_t *s, void *data);
void RS_PerformLogicArray(RetroSprite_t * arr[], int els, void *data);


#define RS_SetAnimation(s, ani) do { \
        s->anim = ani; \
        s->frame = 0; \
    } while(0);

void RS_ResetStats(void);
void RS_PrintStats(void);
int RS_CheckCol(RetroSprite_t *s1, RetroSprite_t *s2);
RetroSprite_t *RS_NewSprite(int x, int y, int z, RetroSpriteGfx_t *g, RetroSpriteLogic_t *l, int anim);
RetroSpriteGfx_t *RS_GetGfx(char *rsfile, char *gfxfile, int w, int h);

int RS_CleanTable(RetroSpriteTable_t *t);
void RS_PrintTable(RetroSpriteTable_t *t);

#endif /* __RETROSPRITE_H__ */
