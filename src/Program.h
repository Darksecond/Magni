#pragma once

#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>
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
    
    void bind() const;
    void unbind() const;
    
    inline void setUniform(const GLchar* name, GLint v0)
        { glUniform1i(uniform(name), v0); }
    inline void setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose=GL_FALSE)
        { glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m)); }
    inline void setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose=GL_FALSE)
        { glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m)); }
    inline void setUniform(const GLchar* name, const glm::vec3& m)
        { glUniform3fv(uniform(name), 1, glm::value_ptr(m)); }
    inline void setUniform(const GLchar* name, const glm::vec4& m)
        { glUniform4fv(uniform(name), 1, glm::value_ptr(m)); }
    inline void setUniform(const GLchar* name, GLfloat v0)
        { glUniform1f(uniform(name), v0); }
};

class ProgramContext
{
    const Program& program;
public:
    ProgramContext(const Program& prog) : program{prog}
    {
        program.bind();
    }
    
    ~ProgramContext()
    {
        program.unbind();
    }
};