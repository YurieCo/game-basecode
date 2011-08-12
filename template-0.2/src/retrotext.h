#ifndef __RETROTEXT_H__
#define __RETROTEXT_H__

#include <stdint.h>

#define RT_LINE_LENGTH  80

typedef char RetroTextString[RT_LINE_LENGTH];

typedef struct rt_charmap_t {
    uint32_t gl_texture;
    char *pixels;
    int fw, fh;
    int tw, th;
}RetroTextCharmap_t;

typedef struct rt_buffer_t {
    RetroTextString *line;
    uint32_t line_n;
}RetroTextBuffer_t;

void RT_DrawString(RetroTextCharmap_t *cm, char *s, int x, int y);
void RT_DrawTextBuffer(RetroTextCharmap_t *cm, RetroTextBuffer_t *tb, int x, int y, int max_y);

RetroTextBuffer_t RT_LoadBuffer(char *filename);
RetroTextCharmap_t RT_LoadSquareCharmap(char *texture, int fw, int fh);


#endif /* __RETROTEXT_H__ */
