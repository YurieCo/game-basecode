#include <stdio.h>

#include "debug.h"

#include "retroassets.h"

#define BUF_LEN 1024
#define FN_LEN 128

RetroCamera_t RA_Camera(uint32_t w, uint32_t h, int min_l, int min_r, int min_t, int min_b,
                        int spr_x, int spr_y, int spr_w, int spr_h)
{
    RetroCamera_t c = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h,
        .min_l = min_l,
        .min_r = min_r,
        .min_t = min_t,
        .min_b = min_b,
        .spr_x = spr_x,
        .spr_y = spr_y,
        .spr_w = spr_w,
        .spr_h = spr_h
    };
    return c;
}

RetroAssets_t * RA_LoadFile(char *filename, RetroSpriteLogic_t *log[])
{
    char buf[BUF_LEN+1];
    RetroAssets_t *as;

    RetroSprite_t *spr;
    RetroWorldScreen_t *wscr;

    int l, x, y, z, w, h, logic, anim;
    char tileset[FN_LEN], tilemap[FN_LEN];
    char sprfile[FN_LEN], imgfile[FN_LEN];

    char s, t;
    int n1, n2, n3, n4, n5, n6, n7, n8, n9;

    FILE *f = fopen(filename, "rb");

    if ( !f )
        return NULL;

    as = calloc(sizeof(RetroAssets_t), 1);
    as->l = log;

    while ( fgets(buf, BUF_LEN, f) )
    {
        retrodebug("# LINE: %s", buf);
        if ( strncmp(buf, "room", 4) == 0 )
        {
            retrodebug("Creating new tables for the new room.\n");
            as->a_n++;
            as->s = realloc(as->s, as->a_n * sizeof(void*));
            as->s[as->a_n-1] = RS_NewTable();

            as->w = realloc(as->w, as->a_n * sizeof(void*));
            as->w[as->a_n-1] = RW_NewTable();
        }
        else if ( sscanf(buf, "screen %d %s %s %s %d %d", &z, tileset, tilemap, imgfile, &w, &h) == 6 )
        {
            retrodebug("Loading new screen...");
            wscr = RW_LoadRetroWorldScreenwTS(tilemap, RW_LoadRetroWorldTilesetwTex(tileset, imgfile, w, h));
            if ( wscr )
            {
//                retrodebug("Loaded wscr: %d x %d\n", wscr->w, wscr->h);
                RW_PushTable(as->w[as->a_n-1], wscr);
                retrodebug(" yay!\n");
            }
            else
            {
                retrodebug(" failed!\n");
            }
        }
        else if ( sscanf(buf, "sprite %d %d %d %s %s %d %d %d %d", &x, &y, &z, sprfile, imgfile, &w, &h, &logic, &anim) == 9 )
        {
            retrodebug("Creating new sprite...");
            spr = RS_NewSprite(x, y, z, RS_GetGfx(sprfile, imgfile, w, h), log[logic], anim);
            if ( spr )
            {
                RS_PushTable(as->s[as->a_n-1], spr);
                retrodebug(" yay!\n");
            }
            else
            {
                retrodebug(" failed!\n");
            }
        }
        else if ( sscanf(buf, "conn %d %d %d %d %d %d %d %d %d", &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9) == 9 )
        {
            retrodebug("Got connection: %d, %d, %d, %d, %d, %d, %d, %d, %d\n", n1, n2, n3, n4, n5, n6, n7, n8, n9);
            RW_PushConnToTable(as->w[as->a_n-1], n1, n2, n3, n4, n5, n6, n7, n8, n9);
        }
        else if ( buf[0] == '#' )
        {
            retrodebug("Ignoring comment.\n");
            // Ignore comment
        }
        else
        {
            retrodebug("Weird line %4d: %s", l, buf);
        }
    }

    fclose(f);

    return as;
}

void RA_EnterConn(RetroSprite_t *s, RetroAssets_t *a, int i)
{
    int old = a->current;
    int deltax = s->x - (a->w[a->current]->c[i].fx * a->w[a->current]->w[s->z]->tw);
    int deltay = s->y - (a->w[a->current]->c[i].fy * a->w[a->current]->w[s->z]->th);

    a->current = a->w[old]->c[i].toscreen;

    RS_PrintTable(a->s[old]);
    retrodebug("Popping...\n");
    RS_PopTable(a->s[old], s);
    RS_PrintTable(a->s[old]);
    RS_PrintTable(a->s[a->current]);
    retrodebug("Pushing...\n");
    RS_PushTable(a->s[a->current], s);
    retrodebug("Pushed...\n");
    RS_PrintTable(a->s[a->current]);
    s->x = (a->w[old]->c[i].tx * a->w[a->current]->w[s->z]->tw) + deltax;
    s->y = (a->w[old]->c[i].ty * a->w[a->current]->w[s->z]->th) + deltay;

    retrodebug("%d, %d\n", deltax, deltay);
}

int RA_SnapToLadder(RetroSprite_t *s, RetroAssets_t *a)
{
    int xft, xlt;
    int yft, ylt;
    int yp, xp;

    xft = (s->x + s->spr->anims[s->anim].box.x ) / a->w[a->current]->w[s->z]->tw;
    if ( s->x + s->spr->anims[s->anim].box.x < 0 )
        xft--;

    xlt = (s->x + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w - 1) / a->w[a->current]->w[s->z]->tw;

    yft = (s->y + s->spr->anims[s->anim].box.y) / a->w[a->current]->w[s->z]->th;
    if ( s->y + s->spr->anims[s->anim].box.y < 0 )
        yft--;

    ylt = (s->y + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h - 1) / a->w[a->current]->w[s->z]->th;

    for(yp=yft;yp<=ylt;yp++)
    {
        for(xp=xft;xp<=xlt;xp++)
        {
            if ( xp < 0 || xp >= a->w[a->current]->w[s->z]->w ||
                 yp < 0 || yp >= a->w[a->current]->w[s->z]->h )
            {
                continue;
            }
            else if (a->w[a->current]->w[s->z]->colmap[yp*a->w[a->current]->w[s->z]->w + xp] == RW_COL_LADDER)
            {
                int delta = ( xp * a->w[a->current]->w[s->z]->tw ) - (s->x + s->spr->anims[s->anim].box.x) + (s->spr->anims[s->anim].box.w / 2);

                if ( delta > 0 && delta < s->spr->anims[s->anim].box.w )
                {
                    s->x += ( xp * a->w[a->current]->w[s->z]->tw ) - (s->x + s->spr->anims[s->anim].box.x); 
                    return 1;
                }

            }
        }
    }
    return 0;
}

int RA_MoveCheckWorldCol(RetroSprite_t *s, RetroAssets_t *a, int x, int y, int sw, int laddercol, int sh)
{
    int i;
    int xv, yv;
    int xp, yp;
    int xft, xlt;
    int yft, ylt;

    int stx, sty;

    while ( s->x != x || s->y != y )
    {
        if ( s->x > x )
            { xv = -1; }
        else if ( s->x < x )
            { xv = 1; }
        else
            { xv = 0; }

        if ( s->y > y )
            { yv = -1; }
        else if ( s->y < y )
            { yv = 1; }
        else
            { yv = 0; }

        xft = (s->x + xv + s->spr->anims[s->anim].box.x ) / a->w[a->current]->w[s->z]->tw;
        if ( s->x + xv + s->spr->anims[s->anim].box.x < 0 )
            xft--;

        xlt = (s->x + xv + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w - 1) / a->w[a->current]->w[s->z]->tw;

        yft = (s->y + yv + s->spr->anims[s->anim].box.y) / a->w[a->current]->w[s->z]->th;
        if ( s->y + yv + s->spr->anims[s->anim].box.y < 0 )
            yft--;

        ylt = (s->y + yv + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h - 1) / a->w[a->current]->w[s->z]->th;

        if ( sw )
        {
            for (i=0;i<a->w[a->current]->w_c;i++)
            {
                if ( a->w[a->current]->c[i].fx <= xft &&
                     a->w[a->current]->c[i].fy <= yft &&
                     a->w[a->current]->c[i].fx + a->w[a->current]->c[i].fw >= xlt &&
                     a->w[a->current]->c[i].fy + a->w[a->current]->c[i].fh >= ylt
                    )
                {
                    RA_EnterConn(s, a, i);
                    return 1;
                }
            }
        }

        for(yp=yft;yp<=ylt;yp++)
        {
            for(xp=xft;xp<=xlt;xp++)
            {
                if ( xp < 0 || xp >= a->w[a->current]->w[s->z]->w ||
                     yp < 0 || yp >= a->w[a->current]->w[s->z]->h )
                {
                    return 1;
                }
                else
                {
                    int ty = a->w[a->current]->w[s->z]->th * yp;
                    int tx = a->w[a->current]->w[s->z]->tw * xp;

                    switch(a->w[a->current]->w[s->z]->colmap[yp*a->w[a->current]->w[s->z]->w + xp])
                    {
                        case RW_COL_NONE:
                            continue;
                            break;
                        case RW_COL_SOLID:
                            return 1;
                            break;
                        case RW_COL_LADDER:
                            if ( laddercol )
                                return 1;
                            break;
                        case RW_COL_SLOPE_SW:   /*  .\  */
                            stx = ( s->x + s->spr->anims[s->anim].box.x - tx );
                            sty = ( s->y + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h - ty );

                            if ( sty > a->w[a->current]->w[s->z]->th )
                                return 1;

                            if ( stx < 0 )
                                break;

                            if ( xv == -1 && a->w[a->current]->w[s->z]->tw - stx + sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_W )
                                {   
                                    y = s->y - 1;
                                    yv = -1; 
                                }   
                                else
                                {   
                                    return 1;
                                }   
                            }

                            if ( yv == 1 && a->w[a->current]->w[s->z]->tw - stx + sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_S )
                                {   
                                    x = s->x + 1;
                                    xv = 1; 
                                }   
                                else
                                {   
                                    return 1;
                                }   
                            }

                            break;
                        case RW_COL_SLOPE_SE:   /*  /.  */
                            stx = ( s->x + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w - tx );
                            sty = ( s->y + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h - ty );

                            if ( sty > a->w[a->current]->w[s->z]->th )
                                return 1;

                            if ( stx > a->w[a->current]->w[s->z]->tw )
                                break;

                            if ( xv == 1 && stx + sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_E )
                                {
                                    y = s->y - 1;
                                    yv = -1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }

                            if ( yv == 1 && stx + sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_S )
                                {
                                    x = s->x - 1;
                                    xv = -1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            
                            break;
                        case RW_COL_SLOPE_NE:   /*  \.  */
                            stx = ( s->x + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w - tx );
                            sty = ( s->y + s->spr->anims[s->anim].box.y - ty );

                            if ( sty > a->w[a->current]->w[s->z]->th )
                                return 1;

                            if ( stx > a->w[a->current]->w[s->z]->tw )
                                break;

                            if ( xv == 1 && stx + a->w[a->current]->w[s->z]->tw - sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_E )
                                {
                                    y = s->y + 1;
                                    yv = 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }

                            if ( yv == -1 && stx + a->w[a->current]->w[s->z]->tw - sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_N )
                                {
                                    x = s->x - 1;
                                    xv = -1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            break;
                        case RW_COL_SLOPE_NW:   /*  ./  */
                            stx = ( s->x + s->spr->anims[s->anim].box.x - tx );
                            sty = ( s->y + s->spr->anims[s->anim].box.y - ty );

                            if ( sty > a->w[a->current]->w[s->z]->th )
                                return 1;

                            if ( stx > a->w[a->current]->w[s->z]->tw )
                                break;

                            if ( xv == -1 && a->w[a->current]->w[s->z]->tw - stx + a->w[a->current]->w[s->z]->th - sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_W )
                                {
                                    y = s->y + 1;
                                    yv = 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }

                            if ( yv == -1 && a->w[a->current]->w[s->z]->tw - stx + a->w[a->current]->w[s->z]->th - sty >= a->w[a->current]->w[s->z]->tw-1 )
                            {
                                if ( sh == RA_SLOPEH_N )
                                {
                                    x = s->x + 1;
                                    xv = 1;
                                }
                                else
                                {
                                    return 1;
                                }
                            }
                            break;

                        case RW_COL_SOLID_N:
                            if ( yv < 1 )
                                break;
                            if ( ty == s->y + s->spr->anims[s->anim].box.y + s->spr->anims[s->anim].box.h )
                                return 1;
                            break;

                        case RW_COL_SOLID_S:
                            if ( yv > -1 )
                                break;
                            if ( ty == s->y + s->spr->anims[s->anim].box.y )
                                return 1;
                            break;

                        case RW_COL_SOLID_E:
                            if ( xv < 1 )
                                break;
                            if ( tx == s->x + s->spr->anims[s->anim].box.x + s->spr->anims[s->anim].box.w )
                                return 1;

                            break;
                        case RW_COL_SOLID_W:
                            if ( xv > -1 )
                                break;
                            if ( tx == s->x + s->spr->anims[s->anim].box.x )
                                return 1;
                            break;

                        default:
                            retrodebug("Unknown tile-type (%d x %d)\n", xft, yft);
                            break;
                    }
                }
            }
        }
        s->x += xv;
        s->y += yv;
    }
    return 0;
}

void RA_CameraAdjust(RetroAssets_t *a, RetroSprite_t *s, RetroCamera_t *c)
{
    if ( s->x < c->x + c->spr_x )
        c->x = s->x - c->spr_x;

    if ( s->y < c->y + c->spr_y )
        c->y = s->y - c->spr_y;

    if ( s->x > c->x + c->spr_x + c->spr_w )
        c->x = s->x - ( c->spr_x + c->spr_w );

    if ( s->y > c->y + c->spr_y + c->spr_h )
        c->y = s->y - ( c->spr_y + c->spr_h );

    if ( c->x < c->min_l )
        c->x = c->min_l;

    if ( c->y < c->min_t )
        c->y = c->min_t;

    if ( c->x > (a->w[a->current]->w[s->z]->w * a->w[a->current]->w[s->z]->tw) - c->min_r - c->w )
        c->x = (a->w[a->current]->w[s->z]->w * a->w[a->current]->w[s->z]->tw) - c->min_r - c->w;

    if ( c->y > (a->w[a->current]->w[s->z]->h * a->w[a->current]->w[s->z]->th) - c->min_b - c->h )
        c->y = (a->w[a->current]->w[s->z]->h * a->w[a->current]->w[s->z]->th) - c->min_b - c->h;
}
