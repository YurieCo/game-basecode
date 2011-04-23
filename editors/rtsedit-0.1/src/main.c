#include <stdio.h>
#include <SDL.h>
#include <GL/gl.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retroworld.h"

#include "glstuff.c"

char BUF[128];

static cur_anim = 0;
static cur_frame = 0;

void draw_infobox(RetroWorldTileset_t *g)
{
    int i;

    sprintf(BUF, "animations: %02d", g->anims_n);
    vDrawString(5, 5, BUF, 1, 1, 1, 1);
    vDrawString(125, 5, "[-]", 1, 0, 0, 1);
    vDrawString(157, 5, "[+]", 0, 1, 0, 1);

    for (i=0; i < g->anims_n; i++)
    {
        sprintf(BUF, "%02d", i);
        if ( cur_anim == i )
            vDrawString(195+((i%25)*32), 5+((i/25)*12), BUF, 1, 1, 1, 1);
        else
            vDrawString(195+((i%25)*32), 5+((i/25)*12), BUF, 0.75, 0.75, 0.75, 1);
    }

    if ( g->anims_n == 0 )
        return;

    sprintf(BUF, "frames    : %02d", g->anims[cur_anim].frames);
    vDrawString(5, 17, BUF, 1, 1, 1, 1);
    vDrawString(125, 17, "[-]", 1, 0, 0, 1);
    vDrawString(157, 17, "[+]", 0, 1, 0, 1);
}

void draw_frames(RetroWorldTileset_t *g)
{
    int i;
    for (i=0;i<g->anims[cur_anim].frames;i++)
    {
        if ( i == cur_frame )
            vDrawColoredQuad(3 + ((i%4) * 36) , 28 + ((i/4) * ((32)+4)), 32+4, 32+4, 1, 0.75, 1, 1);
        vDrawColoredQuad(5 + ((i%4) * 36) , 30 + ((i/4) * ((32)+4)), 32, 32, 0.75, 0.75, 0.75, 1);
        vDrawFrame(g, g->anims[cur_anim].indices[i], 5 + ((i%4) * 36), 30 + ((i/4) * ((32)+4)));
        sprintf(BUF, "%02d", g->anims[cur_anim].indices[i]);
        vDrawString(5 + ((i%4) * 36) , 30 + ((i/4) * ((32)+4)), BUF, 1, 1, 1, 1);
    }
}

void draw_canvas(RetroWorldTileset_t *g)
{
    int i, y;
    int max = (g->tw / g->w) * (g->th / g->h);

    for (i=0;i<max;i++)
    {
        vDrawColoredQuad(368 + ((i%18) * 36), 30 + (i/18) * 36, 32, 32, 1, 0, 1, 1);
        vDrawFrame(g, i, 368 + ((i%18) * 36) , 30 + (i/18) * 36);
    }
    for (y=0;y<g->anims_n;y++)
    {
        vDrawColoredQuad(368 + (((i+y)%18) * 36), 30 + ((i+y)/18) * 36, 32, 32, 1, 0, 1, 1);
        glPushMatrix();
        glTranslated(368 + (((i+y)%18) * 36), 30 + ((i+y)/18) * 36, 0);
        glScalef(2, 2, 2);
        RW_DrawTile(g, 256+y+1);
        glPopMatrix();
    }
}
/*
void draw_buttons(RetroWorldTileset_t *g)
{
    if ( cur_anim >= g->anims_n )
        return;

    if ( g->anims[cur_anim].flags == RS_ANIM_LOOP )
        vDrawString(280, 30, "loop", 1, 1, 1, 1);
    else
        vDrawString(280, 30, "loop", 0.75, 0.75, 0.75, 1);

    if ( g->anims[cur_anim].flags == RS_ANIM_ONCE )
        vDrawString(280, 40, "once", 1, 1, 1, 1);
    else
        vDrawString(280, 40, "once", 0.75, 0.75, 0.75, 1);

    sprintf(BUF, "%02d", g->anims[cur_anim].next_anim);
    vDrawString(300, 50, "[-]", 1, 0, 0, 1);
    vDrawString(324, 50, BUF, 1, 1, 1, 1);
    vDrawString(340, 50, "[+]", 0, 1, 0, 1);

    vDrawString(280, 60, "box x", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].box.x);
    vDrawString(300, 70, "[-]", 1, 0, 0, 1);
    vDrawString(324, 70, BUF, 1, 1, 1, 1);
    vDrawString(340, 70, "[+]", 0, 1, 0, 1);

    vDrawString(280, 80, "box y", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].box.y);
    vDrawString(300, 90, "[-]", 1, 0, 0, 1);
    vDrawString(324, 90, BUF, 1, 1, 1, 1);
    vDrawString(340, 90, "[+]", 0, 1, 0, 1);

    vDrawString(280, 100, "box w", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].box.w);
    vDrawString(300, 110, "[-]", 1, 0, 0, 1);
    vDrawString(324, 110, BUF, 1, 1, 1, 1);
    vDrawString(340, 110, "[+]", 0, 1, 0, 1);

    vDrawString(280, 120, "box h", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].box.h);
    vDrawString(300, 130, "[-]", 1, 0, 0, 1);
    vDrawString(324, 130, BUF, 1, 1, 1, 1);
    vDrawString(340, 130, "[+]", 0, 1, 0, 1);


    vDrawString(280, 140, "offset x", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].offset.x);
    vDrawString(300, 150, "[-]", 1, 0, 0, 1);
    vDrawString(324, 150, BUF, 1, 1, 1, 1);
    vDrawString(340, 150, "[+]", 0, 1, 0, 1);

    vDrawString(280, 160, "offset y", 1, 1, 1, 1);
    sprintf(BUF, "%02d", g->anims[cur_anim].offset.y);
    vDrawString(300, 170, "[-]", 1, 0, 0, 1);
    vDrawString(324, 170, BUF, 1, 1, 1, 1);
    vDrawString(340, 170, "[+]", 0, 1, 0, 1);

}
*/
void draw(RetroWorldTileset_t *g) //, RetroSprite_t *s)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    draw_infobox(g);

    if ( g->anims_n > 0 )
        draw_frames(g);

    draw_canvas(g);

//    draw_buttons(g);

/*    if ( g->anims_n > 0 )
    {
        glPushMatrix();
        glTranslated(280, 200, 0);
        glScalef(2, 2, 2);
        RS_DrawArray(s, 1);
        glPopMatrix();
    }*/

    vDrawString(1024, 5, "save", 1, 1, 1, 1);

    SDL_GL_SwapBuffers();
}

void click(RetroWorldTileset_t *g, SDL_MouseButtonEvent e)
{
    int i;
    if ( e.x >= 125 && e.x < 149 && e.y >= 5 && e.y < 13 && g->anims_n > 0 )
    {
        free(g->anims[g->anims_n-1].indices);
        g->anims_n--;
        g->anims = realloc(g->anims, sizeof(RetroWorldTileani_t) * g->anims_n);
        if ( cur_anim >= g->anims_n ) cur_anim = 0;
    }
    else if ( e.x >= 157 && e.x < 189 && e.y >= 5 && e.y < 13 && g->anims_n < 50 )
    {
        g->anims_n++;
        g->anims = realloc(g->anims, sizeof(RetroWorldTileani_t) * g->anims_n);
        g->anims[g->anims_n-1].frames = 2;
        g->anims[g->anims_n-1].indices = calloc(sizeof(uint16_t), 2);
        for (i=0;i<g->anims_n;i++)
        {
            g->anims[i].frame = 0;
        }
    }
    else if ( g->anims_n > 0 && e.x >= 125 && e.x < 149 && e.y >= 17 && e.y < 25 && g->anims[cur_anim].frames > 1 )
    {
        g->anims[cur_anim].frames--;
        g->anims[cur_anim].indices = realloc(g->anims[cur_anim].indices, sizeof(uint16_t) * g->anims[cur_anim].frames);
    }
    else if ( g->anims_n > 0 && e.x >= 157 && e.x < 189 && e.y >= 17 && e.y < 25 )
    {
        g->anims[cur_anim].frames++;
        g->anims[cur_anim].indices = realloc(g->anims[cur_anim].indices, sizeof(uint16_t) * g->anims[cur_anim].frames);
        g->anims[cur_anim].indices[g->anims[cur_anim].frames-1] = 0;
    }
    else if ( e.x >= 195 && e.x < 1024 && e.y >= 5 && e.y < 29 )
    {
        if ( g->anims_n > (((e.y-5)/12)*25)+(e.x-195)/32 )
            cur_anim = (((e.y-5)/12)*25)+(e.x-195)/32;
    }
    else if ( e.x < 272 && e.y > 30 )
    {
        cur_frame = (((e.y - 30) / 36)*4) + ((e.x - 5) / 36);
    }
    else if ( e.x > 368 && e.y > 30 )
    {
        if ( g->anims_n > 0 && cur_anim < g->anims_n && cur_frame < g->anims[cur_anim].frames )
            g->anims[cur_anim].indices[cur_frame] = ((e.y - 30) / (36))*18 + ((e.x - 368)/ 36);
    }
    else if ( e.x > 1024 && e.y < 20 )
    {
        RW_SaveRetroWorldTileset("out.rts", g);
    }
}

int main(int argc, char *argv[])
{
    int quit = 0;
    int ld = 0;
    int f = 0;
    int n;
    RetroWorldTileset_t *g = calloc(sizeof(RetroWorldTileset_t), 1);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface *screen = SDL_SetVideoMode(1060,900,32,SDL_OPENGL);
    glViewport(0, 0, 1060, 900);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, 1060.0,
            900.0,   0.0,
            -10.0,  10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5, 0.5, 0.5, 1.0);

    if ( argc < 4 )
    {
        printf("Usage: rsedit texture.png tilewidth height\nFor example: rtsedit megaman.png 16 16");
        return -1;
    }
    if ( argc == 5 )
    {
        free(g);
        g = RW_LoadRetroWorldTileset(argv[4]);
    }

    g->pixels = stbi_load(argv[1], (int*)&(g->tw), (int*)&(g->th), &n, 0);

    if ( g->tw == g->th && ( g->tw == 128 || g->tw == 256 || g->tw == 512 || g->tw == 1024 ) )
    {
        printf("Looks good.");
    }
    else
    {
        printf("The texture needs to be a square, sized 128, 256, 512 or 1024 in each dimension.\n");
        return -1;
    }

    glGenTextures(1, &(g->gl_texture));
    glBindTexture(GL_TEXTURE_2D, g->gl_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, g->tw, g->th, 0, GL_RGBA, GL_UNSIGNED_BYTE, g->pixels);

    printf("Loaded '%s', which is %d x %d large.\n", argv[1], g->tw, g->th);
    g->w = atoi(argv[2]);
    g->h = atoi(argv[3]);

    if ( ( g->w != 8 && g->w != 16 && g->w != 32 && g->w != 64 && g->w != 128 ) ||
         ( g->h != 8 && g->h != 16 && g->h != 32 && g->h != 64 && g->h != 128 ) )
    {
        printf("The framewidth and frameheight needs to be in the power of two. (8,16,32,64,128)\n");
        return -1;
    }

    if ( g->w > g->tw || g->h > g->th )
    {
        printf("The frame width/height can't exceed the texture size.\n");
        return -1;
    }

    printf("%d frames per row, %d rows, total %d frames.\n", g->tw / g->w, g->th / g->h, (g->tw / g->w) * (g->th / g->h));

    while ( !quit ) // Inputhandling is done here.
    {
        SDL_Event e;
        while ( SDL_PollEvent(&e) )
        {
            switch(e.type)
            {
            case SDL_QUIT:
                quit++;
                break;
            case SDL_MOUSEBUTTONUP:
                click(g, e.button);
                break;
            case SDL_KEYDOWN:
                break;
            default:
                break;
            }
        }
        int t = (1000/60)-(SDL_GetTicks()-ld);
        if ( t > 0 && t < 1000 ) SDL_Delay(t);
        ld = SDL_GetTicks();

        if ( f % 8 == 0 && g->anims_n > 0 )
            RW_UpdateTileset(g);

        draw(g); //, &s);
        f++;
    }

    return 0;
}
