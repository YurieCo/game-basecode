#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <GL/gl.h>


#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retroworld.h"

#include "glstuff.c"

char BUF[128];

static cur_mode = 0;

static cur_page = 0;
static cur_tile = 0;
static cur_coltile = 0;

static cur_anim = 0;
static cur_frame = 0;

static cam_x = 0, cam_y = 64;

void draw_canvas(RetroWorldTileset_t *g, RetroWorldTileset_t *c2)
{
    RetroWorldTileset_t *drawme;
    if ( cur_mode )
    {
        drawme = c2;
    }
    else
    {
        drawme = g;
    }

    int i, y, c = 0;
    int max = (drawme->tw / drawme->w) * (drawme->th / drawme->h);

    sprintf(BUF, "%02d", cur_page);
    vDrawString(0, 0, "[-]", 1, 0, 0, 1);
    vDrawString(24, 0, BUF, 1, 1, 1, 1);
    vDrawString(40, 0, "[+]", 0, 1, 0, 1);

    vDrawString(100, 0, "[-]", 1, 0, 0, 1);
    vDrawString(124, 0, "top", 1, 1, 1, 1);
    vDrawString(148, 0, "[+]", 0, 1, 0, 1);

    vDrawString(200, 0, "[-]", 1, 0, 0, 1);
    vDrawString(224, 0, "left", 1, 1, 1, 1);
    vDrawString(256, 0, "[+]", 0, 1, 0, 1);

    vDrawString(300, 0, "[-]", 1, 0, 0, 1);
    vDrawString(324, 0, "right", 1, 1, 1, 1);
    vDrawString(364, 0, "[+]", 0, 1, 0, 1);

    vDrawString(400, 0, "[-]", 1, 0, 0, 1);
    vDrawString(424, 0, "bottom", 1, 1, 1, 1);
    vDrawString(472, 0, "[+]", 0, 1, 0, 1);

    vDrawString(500, 0, "colission map", cur_mode, cur_mode, cur_mode, 1);

    for (i=0;i<max;i++)
    {
        c++;
        if ( (c-1) / 30 != cur_page )
            continue;
        vDrawColoredQuad(((c-1) % 30)*34, 16, 32, 32, 1, 0, 1, 1);
        vDrawFrame(drawme, i, ((c-1) % 30)*34, 16);
    }
    for (y=0;y<drawme->anims_n;y++)
    {
        c++;
       if ( (c-1) / 30 != cur_page )
            continue;
        vDrawColoredQuad(((c-1) % 30)*34, 16, 32, 32, 1, 0, 1, 1);
        glPushMatrix();
        glTranslated(((c-1) % 30)*34, 16, 0);
        glScalef(2, 2, 2);
        RW_DrawTile(drawme, 256+y+1);
        glPopMatrix();
    }
    if ( cur_tile > max + y )
        cur_tile = 0;

    glPushMatrix();
    glTranslated(1020, 16, 0);
    glScalef(2, 2, 2);
    RW_DrawTile(drawme, cur_tile);
    glPopMatrix();
}

void draw(RetroWorldTileset_t *g, RetroWorldTileset_t *c, RetroWorldScreen_t *s) //, RetroSprite_t *s)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslated(cam_x, cam_y, 0);
    glScalef(2,2,2);
    RW_DrawScreen(s, 1);
    if ( cur_mode)
        RW_DrawScreenColmap(s, c);
    glPopMatrix();

    vDrawColoredQuad(0, 0, 1060, 64, 0.50, 0.50, 0.50, 1);
    draw_canvas(g, c);

    vDrawString(1024, 5, "save", 1, 1, 1, 1);

    SDL_GL_SwapBuffers();
}

void click(RetroWorldTileset_t *g, RetroWorldScreen_t *s, SDL_MouseButtonEvent e)
{
    int i;
    if ( e.x >= 0 && e.x < 24 && e.y < 15 && cur_page > 0 )
        cur_page--;
    else if ( e.x >= 40 && e.x < 64 && e.y < 15 )
        cur_page ++;
    else if ( e.x > 0 && e.x < 1020 && e.y > 16 && e.y < 48 )
    {
        cur_tile = (cur_page * 30) + (e.x / 34) + 1;
        printf("selected tile: %d\n", cur_tile);
    }
    else if ( e.x >= 1020 && e.y > 16 && e.y < 48 )
        cur_tile = 0;
    else if ( e.y > 64 &&
              e.x >= cam_x && e.x < cam_x + 2*((s->w)*s->tw) &&
              e.y >= cam_y && e.y < cam_y + 2*((s->h)*s->th) )
    {
        int x = (e.x - cam_x) / (2*s->tw);
        int y = (e.y - cam_y) / (2*s->th);
        
        if ( cur_mode )
            s->colmap[y*s->w + x] = cur_tile;
        else
            s->map[y*s->w + x] = cur_tile;
        printf("x = %d, y = %d\n", x, y);
    }
    else if ( e.x >= 100 && e.x < 124 && e.y < 8 )
    {
        printf("Remove top row.\n");
        memmove(s->map, &(s->map[s->w]), sizeof(uint16_t) * s->w * (s->h-1));
        memmove(s->colmap, &(s->colmap[s->w]), sizeof(uint8_t) * s->w * (s->h-1));

        s->h--;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
    }
    else if ( e.x >= 148 && e.x < 172 && e.y < 8 )
    {
        s->h++;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        memmove(&(s->map[s->w]), s->map, sizeof(uint16_t) * s->w * (s->h-1));
        memmove(&(s->colmap[s->w]), s->colmap, sizeof(uint8_t) * s->w * (s->h-1));
        memset(s->map, '\0', sizeof(uint16_t) * s->w);
        memset(s->colmap, '\0', sizeof(uint8_t) * s->w);
        printf("Add top row.\n");
    }

    else if ( e.x >= 200 && e.x < 224 && e.y < 8 )
    {
        for (i=0;i<s->h;i++)
        {
            memmove(&(s->map[i*s->w]) - i, &(s->map[(i*s->w)+1]), sizeof(uint16_t) * (s->w-1));
            memmove(&(s->colmap[i*s->w]) - i, &(s->colmap[(i*s->w)+1]), sizeof(uint8_t) * (s->w-1));
        }
        s->w--;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        printf("Remove left column.\n");
    }
    else if ( e.x >= 256 && e.x < 280 && e.y < 8 )
    {
        s->w++;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        for (i=s->h-1;i>=0;i--)
        {
            memmove(&(s->map[(i*s->w)+1]), &(s->map[i*s->w]) - i, sizeof(uint16_t) * (s->w-1));
            memmove(&(s->colmap[(i*s->w)+1]), &(s->colmap[i*s->w]) - i, sizeof(uint8_t) * (s->w-1));
            s->map[i*s->w] = 0;
            s->colmap[i*s->w] = 0;
        }
        printf("Add left column.\n");
    }
    else if ( e.x >= 300 && e.x < 324 && e.y < 8 )
    {
        for (i=0;i<s->h;i++)
        {
            memmove(&(s->map[i*s->w]) - i, &(s->map[(i*s->w)]), sizeof(uint16_t) * (s->w-1));
            memmove(&(s->colmap[i*s->w]) - i, &(s->colmap[(i*s->w)]), sizeof(uint8_t) * (s->w-1));
        }
        s->w--;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        printf("Remove right column.\n");
    }
    else if ( e.x >= 364 && e.x < 388 && e.y < 8 )
    {
        s->w++;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        for (i=s->h-1;i>=0;i--)
        {
            memmove(&(s->map[(i*s->w)]), &(s->map[i*s->w]) - i, sizeof(uint16_t) * (s->w-1));
            memmove(&(s->colmap[(i*s->w)]), &(s->colmap[i*s->w]) - i, sizeof(uint8_t) * (s->w-1));
            s->map[((i+1)*s->w)-1] = 0;
            s->colmap[((i+1)*s->w)-1] = 0;
        }
        printf("Add right column.\n");
    }
    else if ( e.x >= 400 && e.x < 424 && e.y < 8 )
    {
        s->h--;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        printf("Remove bottom row.\n");
    }
    else if ( e.x >= 472 && e.x < 496 && e.y < 8 )
    {
        s->h++;
        s->map = realloc(s->map, sizeof(uint16_t) * s->w * s->h);
        s->colmap = realloc(s->colmap, sizeof(uint8_t) * s->w * s->h);
        memset(&(s->map[(s->h-1)*s->w]), '\0', sizeof(uint16_t) * s->w);
        memset(&(s->colmap[(s->h-1)*s->w]), '\0', sizeof(uint8_t) * s->w);
        
        printf("Add bottom row.\n");
    }
    else if ( e.x >= 500 && e.x < 612 && e.y < 8 )
    {
        cur_mode = !cur_mode;
        cur_tile = 0;
    }
    else if ( e.x > 1024 && e.y < 13 )
    {
        RW_SaveRetroWorldScreen("out.rtm", s);
    }
}

int main(int argc, char *argv[])
{
    int quit = 0;
    int ld = 0;
    int f = 0;
    int n;
    RetroWorldTileset_t *g;
    RetroWorldTileset_t *c = calloc(sizeof(RetroWorldTileset_t), 1);
    RetroWorldScreen_t *s = calloc(sizeof(RetroWorldScreen_t), 1);

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
    glClearColor(1.0, 0.0, 1.0, 1.0);

    if ( argc < 5 )
    {
        printf("Usage: rsedit texture.png tilewidth height\nFor example: rtsedit megaman.png 16 16");
        return -1;
    }
    g = RW_LoadRetroWorldTileset(argv[4]);

    if ( argc == 6 )
    {
        free(s);
        s = RW_LoadRetroWorldScreen(argv[5]);
    }

    g->pixels = stbi_load(argv[1], (int*)&(g->tw), (int*)&(g->th), &n, 0);
    c->pixels = stbi_load("coltiles.png", &(c->tw), &(c->th), &n, 0);
    c->w = 16;
    c->h = 16;

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

    glGenTextures(1, &(c->gl_texture));
    glBindTexture(GL_TEXTURE_2D, c->gl_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, c->tw, c->th, 0, GL_RGBA, GL_UNSIGNED_BYTE, c->pixels);


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


    s->ts = g;
    if ( argc != 6 )
    {
        s->w = 32;
        s->h = 32;
        s->map = calloc(sizeof(uint16_t), s->w * s->h);
        s->colmap = calloc(sizeof(uint8_t), s->w * s->h);
    }
    s->tw = 16;
    s->th = 16;


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
            case SDL_MOUSEMOTION:
                if ( e.motion.state & SDL_BUTTON(3) )
                {
                    cam_x += e.motion.xrel;
                    cam_y += e.motion.yrel;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if ( e.button.button == SDL_BUTTON_LEFT )
                    click(g, s, e.button);
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

        draw(g,c,s);
        f++;
    }

    return 0;
}
