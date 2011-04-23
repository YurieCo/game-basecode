static const GLubyte charmap[256][32] = {
    ['\'']= { 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0,
              0x00, 0, 0, 0, 0x08, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, },
    ['*'] = { 0x00, 0, 0, 0, 0x10, 0, 0, 0, 0x54, 0, 0, 0, 0x38, 0, 0, 0,
              0xfe, 0, 0, 0, 0x38, 0, 0, 0, 0x54, 0, 0, 0, 0x10, 0, 0, 0, },
    [','] = { 0x30, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x00, 0, 0, 0,
              0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, },

    ['-'] = { 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0xfe, 0, 0, 0,
              0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, },
    ['+'] = { 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0xfe, 0, 0, 0,
              0x10, 0, 0, 0, 0x10, 0, 0, 0, 0x10, 0, 0, 0, 0x00, 0, 0, 0, },
    ['['] = { 0x3c, 0, 0, 0, 0x20, 0, 0, 0, 0x20, 0, 0, 0, 0x20, 0, 0, 0,
              0x20, 0, 0, 0, 0x20, 0, 0, 0, 0x3c, 0, 0, 0, 0x00, 0, 0, 0, },
    [']'] = { 0x3c, 0, 0, 0, 0x04, 0, 0, 0, 0x04, 0, 0, 0, 0x04, 0, 0, 0,
              0x04, 0, 0, 0, 0x04, 0, 0, 0, 0x3c, 0, 0, 0, 0x00, 0, 0, 0, },


    ['.'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x00, 0, 0, 0,
              0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, 0x00, 0, 0, 0, },
    ['0'] = { 0x00, 0, 0, 0, 0x38, 0, 0, 0, 0x64, 0, 0, 0, 0xc6, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x4c, 0, 0, 0, 0x38, 0, 0, 0, },
    ['1'] = { 0x00, 0, 0, 0, 0xfc, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0,
              0x30, 0, 0, 0, 0x30, 0, 0, 0, 0x70, 0, 0, 0, 0x30, 0, 0, 0, },
    ['2'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xe0, 0, 0, 0, 0x78, 0, 0, 0,
              0x3c, 0, 0, 0, 0x06, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['3'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0,
              0x3c, 0, 0, 0, 0x18, 0, 0, 0, 0x0c, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['4'] = { 0x00, 0, 0, 0, 0x0c, 0, 0, 0, 0x0c, 0, 0, 0, 0xfe, 0, 0, 0,
              0xcc, 0, 0, 0, 0x6c, 0, 0, 0, 0x3c, 0, 0, 0, 0x1c, 0, 0, 0, },
    ['5'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0,
              0x06, 0, 0, 0, 0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['6'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0x60, 0, 0, 0, 0x3c, 0, 0, 0, },
    ['7'] = { 0x00, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0, 0x30, 0, 0, 0,
              0x18, 0, 0, 0, 0x0c, 0, 0, 0, 0xc6, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['8'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['9'] = { 0x00, 0, 0, 0, 0x78, 0, 0, 0, 0x0c, 0, 0, 0, 0x06, 0, 0, 0,
              0x7e, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['a'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfe, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x6c, 0, 0, 0, 0x38, 0, 0, 0, },
    ['b'] = { 0x00, 0, 0, 0, 0xfc, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0xfc, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['c'] = { 0x00, 0, 0, 0, 0x3c, 0, 0, 0, 0x66, 0, 0, 0, 0xc0, 0, 0, 0,
              0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0x66, 0, 0, 0, 0x3c, 0, 0, 0, },
    ['d'] = { 0x00, 0, 0, 0, 0xf8, 0, 0, 0, 0xcc, 0, 0, 0, 0xc6, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xcc, 0, 0, 0, 0xf8, 0, 0, 0, },
    ['e'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0,
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['f'] = { 0x00, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0,
              0xfc, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfe, 0, 0, 0, },
    ['g'] = { 0x00, 0, 0, 0, 0x3e, 0, 0, 0, 0x66, 0, 0, 0, 0xc6, 0, 0, 0,
              0xce, 0, 0, 0, 0xc0, 0, 0, 0, 0x60, 0, 0, 0, 0x3e, 0, 0, 0, },
    ['h'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0xfe, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['i'] = { 0x00, 0, 0, 0, 0x7e, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0,
              0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['j'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0x06, 0, 0, 0, 0x06, 0, 0, 0, 0x06, 0, 0, 0, 0x1e, 0, 0, 0, },
    ['k'] = { 0x00, 0, 0, 0, 0xce, 0, 0, 0, 0xdc, 0, 0, 0, 0xf8, 0, 0, 0,
              0xf0, 0, 0, 0, 0xd8, 0, 0, 0, 0xcc, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['l'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0,
              0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, },
    ['m'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xd6, 0, 0, 0,
              0xfe, 0, 0, 0, 0xfe, 0, 0, 0, 0xee, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['n'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xce, 0, 0, 0, 0xde, 0, 0, 0,
              0xfe, 0, 0, 0, 0xf6, 0, 0, 0, 0xe6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['o'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['p'] = { 0x00, 0, 0, 0, 0xc0, 0, 0, 0, 0xc0, 0, 0, 0, 0xfc, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['q'] = { 0x00, 0, 0, 0, 0x7a, 0, 0, 0, 0xcc, 0, 0, 0, 0xde, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0x7c, 0, 0, 0, },
    ['r'] = { 0x00, 0, 0, 0, 0xce, 0, 0, 0, 0xdc, 0, 0, 0, 0xf8, 0, 0, 0,
              0xce, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xfc, 0, 0, 0, },
    ['s'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0x06, 0, 0, 0,
              0x7c, 0, 0, 0, 0xc0, 0, 0, 0, 0xcc, 0, 0, 0, 0x78, 0, 0, 0, },
    ['t'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0,
              0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x7e, 0, 0, 0, },
    ['u'] = { 0x00, 0, 0, 0, 0x7c, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0,
              0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['v'] = { 0x00, 0, 0, 0, 0x10, 0, 0, 0, 0x38, 0, 0, 0, 0x7c, 0, 0, 0,
              0xee, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['w'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xee, 0, 0, 0, 0xfe, 0, 0, 0,
              0xfe, 0, 0, 0, 0xd6, 0, 0, 0, 0xc6, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['x'] = { 0x00, 0, 0, 0, 0xc6, 0, 0, 0, 0xee, 0, 0, 0, 0x7c, 0, 0, 0,
              0x38, 0, 0, 0, 0x7c, 0, 0, 0, 0xee, 0, 0, 0, 0xc6, 0, 0, 0, },
    ['y'] = { 0x00, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0, 0x18, 0, 0, 0,
              0x3c, 0, 0, 0, 0x66, 0, 0, 0, 0x66, 0, 0, 0, 0x66, 0, 0, 0, },
    ['z'] = { 0x00, 0, 0, 0, 0xfe, 0, 0, 0, 0xe0, 0, 0, 0, 0x70, 0, 0, 0,
              0x38, 0, 0, 0, 0x1c, 0, 0, 0, 0x0e, 0, 0, 0, 0xfe, 0, 0, 0, },
};
static void vDrawString(int x, int y, const char *string, float r, float g, float b, float a)
{
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        glColor4f(r, g, b, a);
        glRasterPos2i(x, y+8);
        glPixelTransferi(GL_UNPACK_ALIGNMENT, 4);
        while (*string != '\0')
        {
            glBitmap(8, 8, 0, 0, 8, 0, (const GLubyte *) &charmap[(int) *string]);
            string++;
        }
    glPopMatrix();
}

static void vDrawColoredQuad(float x, float y, float w, float h, float r, float g, float b, float a)
{
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(x,y,0.0);
    GLfloat vcoords[] = {
        0.0, 0.0,
        w, 0.0,
        w, h,
        0.0, h
    };
    glColor4f(r,g,b,a);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}

static void vDrawTexturedQuad(float x, float y, float w, float h, GLuint id)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(x+(w/2),y+(h/2),0.0);
    GLfloat vcoords[] = {
        -(w/2), -(h/2),
        w/2, -(h/2),
        w/2, h/2,
        -(w/2), h/2
    };
    GLfloat tcoords[8];

    tcoords[0] = 0.0;   tcoords[1] = 0.0;
    tcoords[2] = 1.0;   tcoords[3] = 0.0;
    tcoords[4] = 1.0;   tcoords[5] = 1.0;
    tcoords[6] = 0.0;   tcoords[7] = 1.0;

    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, id);
    glVertexPointer(2,GL_FLOAT,0,vcoords);
    glTexCoordPointer(2,GL_FLOAT,0,tcoords);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glPopMatrix();
}


static void vDrawFrame(RetroSpriteGfx_t *g, int id, int x, int y)
{
    float fw = (float)g->w / (float)g->tw;
    float fh = (float)g->h / (float)g->th;

    int fpw = g->tw / g->w;
    int fph = g->th / g->h;

    int fid = id;
    float height = (float)g->h / (float)g->w;

    float vcoords[] = {
        0, 0,
        64, 0,
        64, 64*height,
        0, 64*height,
    };

    float tcoords[8];

    tcoords[0] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[1] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[2] = fw  + ( fw * ( fid % fpw ) ); tcoords[3] = 0.0 + ( fh * ( fid / fph ) );
    tcoords[4] = fw  + ( fw * ( fid % fpw ) ); tcoords[5] = fh  + ( fh * ( fid / fph ) );
    tcoords[6] = 0.0 + ( fw * ( fid % fpw ) ); tcoords[7] = fh  + ( fh * ( fid / fph ) );


    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, g->gl_texture);
    glVertexPointer(2, GL_FLOAT, 0, vcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, tcoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPopMatrix();
}

