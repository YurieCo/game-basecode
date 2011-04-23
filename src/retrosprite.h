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
    uint32_t gl_texture;
    char *pixels;

    uint16_t w, h;   // Frame width / height
    uint32_t tw, th; // Texture width / height

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

#define RS_FLAGS_FLIP_V 1
#define RS_FLAGS_FLIP_H 2

typedef struct retrospritelogic_t {
    void (*l[256])();
}RetroSpriteLogic_t;


typedef struct retrosprite_t {
    int x, y, z; // Position

    RetroSpriteGfx_t *spr; // Reference to a RetroSpriteGfx_t
    uint32_t flags;
    uint16_t anim;
    uint16_t frame;

    RetroSpriteLogic_t *logic; // Reference to a RetroSpriteLogic_t
}RetroSprite_t;


void RS_DrawArray(RetroSprite_t *arr, uint16_t elements);
void RS_DrawBoxArray(RetroSprite_t *arr, uint16_t elements);
void RS_UpdateSprite(RetroSprite_t *s);

void RS_PerformLogicArray(RetroSprite_t *arr, uint16_t elements, void *data);

void RS_SaveRetroSpriteGfx(char *filename, RetroSpriteGfx_t *g);
RetroSpriteGfx_t * RS_LoadRetroSpriteGfx(char *filename);


#define RS_UpdateArray(arr, els) do { \
        int i; \
        for (i=0;i<els;i++) { \
            RS_UpdateSprite(&arr[i]); \
        } \
    } while(0);

#define RS_PerformLogic(s, d) do { \
        if ( s->logic->l[s->anim] ) (s->logic->l[s->anim])(s, d); \
    } while(0);

#define RS_PerformLogicArray(arr, els, d)  do { \
        int i; \
        for (i=0;i<els;i++) { \
            RS_PerformLogic(&arr[i], d); \
        } \
    } while(0);


void RS_ResetStats(void);
void RS_PrintStats(void);


#endif /* __RETROSPRITE_H__ */
