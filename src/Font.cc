#include "Font.h"

#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace Ymir;

//http://branan.github.com/gltext/
//https://github.com/branan/gltext/blob/master/gltext.cpp

std::unique_ptr<Font> Font::fontFromFile(const std::string& file, int sz, Program& p)
{
    char* data = nullptr;
    std::ifstream stream(file, std::istream::in);
    std::streampos cur = stream.tellg();
    stream.seekg(0, std::ios::end);
    std::streampos size = stream.tellg();
    stream.seekg(cur);
    data = new char[size];
    stream.read(data, size);
    Font* fnt = new Font{512, 512, (unsigned char*)data, sz, p};
    delete[] data;
    return std::unique_ptr<Font>(fnt);
}


//---------------------------
// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

static const unsigned char utf8d[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
	8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
	0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
	0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
	0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
	1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
	1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
	1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

static unsigned int decutf8(unsigned int* state, unsigned int* codep, unsigned int byte)
{
	unsigned int type = utf8d[byte];
	*codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);
	*state = utf8d[256 + *state*16 + type];
	return *state;
}
//---------------------------


Font::Font(int cache_w, int cache_h, const unsigned char* data, int s, Program& p) : tex_width(cache_w), tex_height(cache_h), size(s), program(p)
{
    it_width = 1.0f / cache_w;
	it_height = 1.0f / cache_h;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, cache_w, cache_h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //create VAO, VBO and IBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    //load font from data
    if (!stbtt_InitFont(&font, data, 0))
    {
        throw std::runtime_error("could not create font from data");
    }

    //load font metrics
    int ascent, descent, fh, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
	fh = ascent - descent;
	ascender = (float)ascent / (float)fh;
	descender = (float)descent / (float)fh;
	line_height = (float)(fh + lineGap) / (float)fh;

    float scale = stbtt_ScaleForPixelHeight(&font, size);
    int x0, x1, y0, y1;
    stbtt_GetFontBoundingBox(&font, &x0, &x1, &y0, &y1);
    int gw = x1 - x0;
    int gh = y1 - y0;
    gw *= scale;
    gh *= scale;
    int max_glyphs = (tex_width / gw)*(tex_height / gh);

    glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(GLfloat)*max_glyphs, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort)*max_glyphs, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(program.attrib("vert"));
    glEnableVertexAttribArray(program.attrib("vertTexCoord"));
    glVertexAttribPointer(program.attrib("vert"), 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    glVertexAttribPointer(program.attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font()
{
    if(tex)
        glDeleteTextures(1, &tex);
    if(vbo)
        glDeleteBuffers(1, &vbo);
    if(ibo)
        glDeleteBuffers(1, &ibo);
    if(vao)
        glDeleteVertexArrays(1, &vao);
}

//TODO vec2 -> ivec2?
void Font::setPosition(glm::vec2& pos)
{
    //TODO
}

void Font::print(const std::string& text)
{
    const char* s = text.c_str();
    unsigned int codepoint;
	unsigned int state = 0;

    ProgramContext p{program};
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(vao);

    //gltextUniform2i(FontSystem::instance().scale_loc, self->window_w, self->window_h);
    //gltextUniform3f(FontSystem::instance().col_loc, self->pen_r, self->pen_g, self->pen_b);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (; *s; ++s)
	{
		if (decutf8(&state, &codepoint, *(unsigned char*)s)) continue;

        //find glyph
        //unsigned glyph = ...

        //gltextUniform2i(FontSystem::instance().pos_loc, self->pen_x+positions[i].x_offset, self->pen_+positions[i].y_offset);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid*)(glyph*GLYPH_IDX_SIZE));
        //self->pen_x += positions[i].x_advance >> 6;
        //self->pen_y += positions[i].y_advance >> 6;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}
