#pragma once

#include <GLEW/glew.h>
#include <memory>
#include <vector>

#include "Shader.h"

class Program
{
    GLuint _object;
public:
    Program(const std::vector<Shader>& shaders);
    Program(Program&& other);
    Program(const Program&) = delete;
    ~Program();
    
    Program& operator=(Program&& other);
    Program& operator=(const Program&) = delete;
    
    GLuint object() const;
    
    GLint attrib(const GLchar* attribName) const;
    GLint uniform(const GLchar* uniformName) const;
};

class ProgramContext
{
public:
    ProgramContext(const Program& program)
    {
        glUseProgram(program.object());
    }
    
    ~ProgramContext()
    {
        glUseProgram(0);
    }
};