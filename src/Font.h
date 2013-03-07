#pragma once

#include "stb_truetype.h"
#include "Program.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/glfw.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32

#include <GLM/glm.hpp>
#include <string>
#include <memory>



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
