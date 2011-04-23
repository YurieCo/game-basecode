AM_CFLAGS = -Wall
bin_PROGRAMS = rtsedit
rtsedit_SOURCES = main.c retroworld.c stb_image.c
rtsedit_LDADD = $(GL_LIBS) \
               $(SDL_LIBS)

EXTRA_DIST = retroworld.h glstuff.c

#rtsedit_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
rtsedit_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(sqlite3_CFLAGS)
