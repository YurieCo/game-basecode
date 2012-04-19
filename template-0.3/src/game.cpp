#include <iostream>
#include <cstdio>

#ifdef TARGET_SDL
#include <SDL.h>
#endif

#include "renderer.h"
#include "audio.h"
#include "resourcemanager.h"
#include "game.h"
#include "image.h"
#include "drawable.h"
#include "tileset.h"
#include "tilemap.h"
#include "resource.h"
#include "font.h"
#include "sprite.h"
#include "input.h"

game::game() : c(0, 0, 480, 320)
{
    updateno = 0;

    in = new input();
    r = new renderer(480, 320);
    rm = new resourcemanager(this);
    a = new audio();
    f = new font(rm->getImage("charmap3.png"), this, 8, 8);

    std::cout << "Made game" << std::endl;
}

game::~game()
{
    delete(rm);
    delete(r);
    delete(f);
    delete(in);
    delete(a);
}

void
game::render()
{
    r->clear();

    for (uint32_t i=0; i < assets.size(); i++)
    {
        assets[i]->render();
    }

    r->swap();
}

int
game::update()
{
    updateno++;

#ifdef TARGET_SDL
    if ( updateno % 60 == 0 )
    {
        char caption[128];
        sprintf(caption, "Assets: %lu", assets.size());
        SDL_WM_SetCaption(caption, NULL);
    }
#endif

    for (uint32_t i=0; i < assets.size(); i++)
    {
        assets[i]->update();
    }


    this->setReload(0);

    return 0;
}

int
game::collides(float x, float y, float w, float h, drawable * me)
{
    int ret = 0;
    for (uint32_t i=0; i < assets.size(); i++)
    {
        if ( assets[i] == me )
            continue;

        ret |= assets[i]->collidesWith(x, y, w, h, me);

    }
    return ret;
}

void
game::setReload( int val )
{
    this->reload = val;
}

int
game::getReload()
{
    return reload;
}
