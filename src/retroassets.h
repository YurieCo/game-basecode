#ifndef __RETROASSETS_H__
#define __RETROASSETS_H__

#include <stdint.h>

#include "retroworld.h"
#include "retrosprite.h"
#include "retrocommon.h"


typedef struct retroassets_t {    
    RetroSpriteTable_t **s;
    RetroWorldScreenTable_t **w;

    // Table count.
    uint32_t a_n;

    // The index of the tables we currently want to focus on.
    uint32_t current;
}RetroAssets_t;

RetroAssets_t * RA_LoadFile(char *filename, RetroSpriteLogic_t *log[]);

#endif /* __RETROASSETS_H__ */
