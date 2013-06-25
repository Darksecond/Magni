#pragma once

#include "Program.h"
#include "Texture.h"

#include <string>
#include <GLM/glm.hpp>
#ifdef __APPLE__
    #include <GLFW/GLFW.h>
    #include <GLEW/glew.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32

class text
{
    text(const text&) = delete;
    text(text&&) = delete;
    text& operator=(const text&) = delete;
    text& operator=(text&&) = delete;
    
    std::string _text;
    int _size;
    
    glm::vec2 _bottom_left;
    glm::vec2 _top_right;
    
    GLuint _vbo, _uv_vbo, _vao;
    size_t _verts_count;
    
    void generate();
public:
    text(const glm::vec2& pos, const std::string& text, const int size);
    ~text();
    
    void draw(Ymir::Program& program) const;
    
    void set_text(const std::string& t);
};