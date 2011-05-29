#ifndef __RETROSPRITE_H__
#define __RETROSPRITE_H__

#include <stdint.h>

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
#define RS_ANIM_MANUAL  2

typedef struct retrospritelogic_t {
    void *standard;
    void *attack;
    void *die;
}RetroSpriteLogic_t;


typedef struct retrosprite_t {
    int x, y, z; // Position

    RetroSpriteGfx_t *spr; // Reference to a RetroSpriteGfx_t
    uint32_t flags;
    uint16_t anim;
    uint16_t frame;

    RetroSpriteLogic_t *logic; // Reference to a RetroSpriteLogic_t
    uint32_t logic_cur;
}RetroSprite_t;


void RS_DrawArray(RetroSprite_t *arr, uint16_t elements);
void RS_UpdateArray(RetroSprite_t *arr, uint16_t elements);
void RS_SaveRetroSpriteGfx(char *filename, RetroSpriteGfx_t *g);
RetroSpriteGfx_t * RS_LoadRetroSpriteGfx(char *filename);

#endif /* __RETROSPRITE_H__ */
