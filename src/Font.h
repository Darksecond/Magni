#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw.h>
#include <GLM/glm.hpp>
#include <string>
#include <memory>

#include "stb_truetype.h"
#include "Program.h"

namespace Ymir
{
    class Font
    {
        GLuint tex;
        GLuint vao, vbo, ibo;
        int tex_width, tex_height;
        float it_width, it_height;
        
        stbtt_fontinfo font;
        float ascender;
        float descender;
        float line_height;
        
        int size;
        Program& program;
    public:
        static std::unique_ptr<Font> fontFromFile(const std::string& file, int size, Program& p);
        
        Font(int cache_w, int cache_h, const unsigned char* data, int size, Program& p);
        ~Font();
        
        Font(Font&) = delete;
        Font& operator=(Font&) = delete;
        
        void setPosition(glm::vec2& pos);
        void setSize(int size);
        void print(const std::string& text);
    };
};