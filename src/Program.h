#pragma once

#include <GLEW/glew.h>
#include <memory>
#include <vector>

#include "Shader.h"

class Program
{
    GLuint _object;
public:
    Program(const std::vector<std::shared_ptr<Shader>>& shaders);
    Program(Program const&) = delete;
    ~Program();
    
    void operator=(Program const&) = delete;
    
    GLuint object() const;
    
    GLint attrib(const GLchar* attribName) const;
    GLint uniform(const GLchar* uniformName) const;
};