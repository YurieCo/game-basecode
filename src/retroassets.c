#include <stdio.h>

#include "retroassets.h"

#define BUF_LEN 1024
#define FN_LEN 128

RetroAssets_t * RA_LoadFile(char *filename, RetroSpriteLogic_t *log[])
{
    char buf[BUF_LEN+1];
    RetroAssets_t *as;

    RetroSprite_t *spr;
    RetroWorldScreen_t *wscr;

    int l, x, y, z, w, h, logic, anim;
    char tileset[FN_LEN], tilemap[FN_LEN];
    char sprfile[FN_LEN], imgfile[FN_LEN];

    FILE *f = fopen(filename, "rb");

    if ( !f )
        return NULL;

    as = calloc(sizeof(RetroAssets_t), 1);

    while ( fgets(buf, BUF_LEN, f) )
    {
        printf("# LINE: %s", buf);
        if ( strncmp(buf, "room", 4) == 0 )
        {
            printf("Creating new tables for the new room.\n");
            as->a_n++;
            as->s = realloc(as->s, as->a_n * sizeof(void*));
            as->s[as->a_n-1] = RS_NewTable();

            as->w = realloc(as->w, as->a_n * sizeof(void*));
            as->w[as->a_n-1] = RW_NewTable();
        }
        else if ( sscanf(buf, "screen %d %s %s %s %d %d", &z, tileset, tilemap, imgfile, &w, &h) == 6 )
        {
            printf("Loading new screen...");
            wscr = RW_LoadRetroWorldScreenwTS(tilemap, RW_LoadRetroWorldTilesetwTex(tileset, imgfile, w, h));
            if ( wscr )
            {
//                printf("Loaded wscr: %d x %d\n", wscr->w, wscr->h);
                RW_PushTable(as->w[as->a_n-1], wscr);
                printf(" yay!\n");
            }
            else
            {
                printf(" failed!\n");
            }
        }
        else if ( sscanf(buf, "sprite %d %d %d %s %s %d %d %d %d", &x, &y, &z, sprfile, imgfile, &w, &h, &logic, &anim) == 9 )
        {
            printf("Creating new sprite...");
            spr = RS_NewSprite(x, y, z, RS_GetGfx(sprfile, imgfile, w, h), log[logic], anim);
            if ( spr )
            {
                RS_PushTable(as->s[as->a_n-1], spr);
                printf(" yay!\n");
            }
            else
            {
                printf(" failed!\n");
            }
        }
        else if ( buf[0] == '#' )
        {
            printf("Ignoring comment.\n");
            // Ignore comment
        }
        else
        {
            printf("Weird line %4d: %s", l, buf);
        }
    }

    fclose(f);

    return as;
}
