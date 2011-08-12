#include <stdio.h>
#include <SDL.h>
#include <GL/gl.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retrotext.h"
#include "retrosprite.h"
#include "retroworld.h"
#include "retroassets.h"
#include "retrolights.h"

RetroSpriteLogic_t *l[] = {
    NULL
};


void draw(RetroAssets_t *a)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if ( a->current < a->a_n )
	{
	    glPushMatrix();
	    glTranslated(a->c.x * -1, a->c.y * -1, 0);
	    RW_DrawScreen(a->w[a->current]->w[0], 0);
	    RS_DrawArray(a->s[a->current]->s, a->s[a->current]->s_n);
	    if ( a->w[a->current]->w_n > 1 )
	    {
	        RW_DrawScreen(a->w[a->current]->w[1], 0);
	    }
	    RL_DrawLights(a, 0);
	    glPopMatrix();
	}

    SDL_GL_SwapBuffers();
}

int main(int argc, char *argv[])
{
    int quit = 0, ld = 0, f = 0;
    RetroAssets_t *a;

    /*
        Setting up video
    */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Surface *screen = SDL_SetVideoMode(960,640,32,SDL_OPENGL);
    glViewport(0, 0, 960, 640);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(  0.0, 480.0,
            320.0,   0.0,
            -10.0,  10.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0, 0, 0, 1.0);

    a = RA_LoadFile("share/assets", l);
    a->c = RA_Camera(480, 320, 16, 16, 16, 16, 160, 96, 160, 128);

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
			case SDL_KEYDOWN:
				if ( e.key.keysym.sym == 'p' )
					SDL_Delay(10000);
				break;
            default:
                break;
            }
        }
        int t = (1000/60)-(SDL_GetTicks()-ld);
        if ( t > 0 && t < 1000 ) SDL_Delay(t);
        ld = SDL_GetTicks();

        if ( a->current < a->a_n )
		{
			RS_PerformLogicArray(a->s[a->current]->s, a->s[a->current]->s_n, a);
	        while ( RS_CleanTable(a->s[a->current]) )
	        {
	            retrodebug("Freed dead sprite.\n");
	        }

	        if ( f % 6 == 0 )
	        {
	            RS_UpdateArray(a->s[a->current]->s, a->s[a->current]->s_n);
	            RW_UpdateTileset(a->w[a->current]->w[0]->ts);
	        }
		}

        draw(a);
        f++;
    }
    (void)argc; (void)argv; (void)screen;

    return 0;
}
