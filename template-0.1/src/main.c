#include <stdio.h>
#include <SDL.h>
#include <GL/gl.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retrotext.h"
#include "retrosprite.h"
#include "retroworld.h"

struct assets
{
    void *wat;
};

void draw(struct assets *a)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapBuffers();
}

int main(int argc, char *argv[])
{
    int quit = 0, ld = 0, f = 0;
    struct assets a;
    /*
        Setting up video
    */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface *screen = SDL_SetVideoMode(1024,768,32,SDL_OPENGL);
    glViewport(0, 0, 1024, 768);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, 512.0,
            384.0,   0.0,
            -10.0,  10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0, 0, 0, 1.0);


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
            default:
                break;
            }
        }
        int t = (1000/60)-(SDL_GetTicks()-ld);
        if ( t > 0 && t < 1000 ) SDL_Delay(t);
        ld = SDL_GetTicks();

        draw(&a);
        f++;
    }
    (void)argc; (void)argv; (void)screen;

    return 0;
}
