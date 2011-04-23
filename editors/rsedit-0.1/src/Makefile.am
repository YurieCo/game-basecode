AM_CFLAGS = -Wall
bin_PROGRAMS = rsedit
rsedit_SOURCES = main.c retrosprite.c stb_image.c
rsedit_LDADD = $(GL_LIBS) \
               $(SDL_LIBS)

EXTRA_DIST = retrosprite.h glstuff.c

#rsedit_LDADD += -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/
rsedit_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) $(SDL_image_CFLAGS) $(SDL_mixer_CFLAGS) $(sqlite3_CFLAGS)
