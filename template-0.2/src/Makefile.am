AM_CFLAGS = -Wall 
bin_PROGRAMS = game
game_SOURCES = main.c retrosprite.c retrotext.c retroworld.c retrowsfuncs.c retrocommon.c retroassets.c \
			   retrolights.c retrosound.c stb_image.c \
			   megaman.c debug.c boss1.c projectile.c lamp.c
game_LDADD = $(OPENAL_LIBS) \
			 $(GL_LIBS) \
             $(SDL_LIBS)

EXTRA_DIST = retrosprite.h retrotext.h retroworld.h retrowsfuncs.h retro.h retrocommon.h retroassets.h retrolights.h retrosound.h megaman.h boss1.h projectile.h lamp.h

game_CFLAGS = -g $(OPENAL_CFLAGS) $(GL_CFLAGS) $(SDL_CFLAGS) 
