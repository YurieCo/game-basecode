AM_CFLAGS = -Wall -ggdb
bin_PROGRAMS = rtmedit
rtmedit_SOURCES = main.c retroworld.c stb_image.c
rtmedit_LDADD = $(GL_LIBS) \
               $(SDL_LIBS)

EXTRA_DIST = retroworld.h glstuff.c

#rtmedit_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
rtmedit_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(sqlite3_CFLAGS)
