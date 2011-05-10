#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <GL/gl.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#include "retrotext.h"

void RT_DrawCharacter(RetroTextCharmap_t *cm, char c)
{
    float fw = (float)cm->fw / (float)cm->tw;
    float fh = (float)cm->fh / (float)cm->th;
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    int fpw = cm->tw / cm->fw;
    int fph = cm->th / cm->fh;

    float vcoords[] = {
        0, 0,
        cm->fw, 0,
        cm->fw, cm->fh,
        0, cm->fh,
    };

    float tcoords[8];

    tcoords[0] = 0.0 + ( fw * ( c % fpw ) ); tcoords[1] = 0.0 + ( fh * ( c / fph ) );
    tcoords[2] = fw  + ( fw * ( c % fpw ) ); tcoords[3] = 0.0 + ( fh * ( c / fph ) );
    tcoords[4] = fw  + ( fw * ( c % fpw ) ); tcoords[5] = fh  + ( fh * ( c / fph ) );
    tcoords[6] = 0.0 + ( fw * ( c % fpw ) ); tcoords[7] = fh  + ( fh * ( c / fph ) );


    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, cm->gl_texture);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);

    glPushMatrix();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();
}

void RT_DrawString(RetroTextCharmap_t *cm, char *s, int x, int y)
{
    glPushMatrix();
    glTranslated(x, y, 0);
    while (*s)
    {
        RT_DrawCharacter(cm, *s);
        glTranslated(cm->fw, 0, 0);
        s++;
    }
    glPopMatrix();
}

void RT_DrawTextBuffer(RetroTextCharmap_t *cm, RetroTextBuffer_t *tb, int x, int y, int max_y)
{
    int i;
    for(i=0;i<tb->line_n;i++)
    {
        if ( y + ((i+1)*cm->fh) <= 0 )
            continue;
        if ( y + (i*cm->fh) > max_y )
            return;
        RT_DrawString(cm, tb->line[i], x, y + (i * cm->fh));
    }
}

RetroTextBuffer_t RT_LoadBuffer(char *filename)
{
    RetroTextBuffer_t b;

    b.line_n = 0;
    b.line = malloc((b.line_n+1)*sizeof(RetroTextString));

    FILE *f = fopen(filename, "r");

    while ( fgets(b.line[b.line_n], RT_LINE_LENGTH-1, f)  )
    {
        b.line_n++;
        b.line = realloc(b.line, (b.line_n+1)*sizeof(RetroTextString));
    }

    int i;
    for (i=0;i<b.line_n;i++)
    {
        printf("%02d: %s", i, b.line[i]);
    }

    return b;
}

RetroTextCharmap_t RT_LoadSquareCharmap(char *texture, int fw, int fh)
{
    RetroTextCharmap_t t;
    int n;

    glEnable(GL_TEXTURE_2D);
    t.pixels = stbi_load(texture, &t.tw, &t.th, &n, 0);
    glGenTextures(1, &t.gl_texture);
    glBindTexture(GL_TEXTURE_2D, t.gl_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, t.tw, t.th, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.pixels);

    t.fw = fw;
    t.fh = fh;

    return t;
}
