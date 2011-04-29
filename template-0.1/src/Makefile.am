AM_CFLAGS = -Wall
bin_PROGRAMS = template
template_SOURCES = main.c retrosprite.c retrotext.c retroworld.c retrowsfuncs.c stb_image.c
template_LDADD = $(GL_LIBS) \
               $(SDL_LIBS)

EXTRA_DIST = retrosprite.h retrotext.h retroworld.h retrowsfuncs.h

template_CFLAGS = -g $(GL_CFLAGS) $(SDL_CFLAGS) 
