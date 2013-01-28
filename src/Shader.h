#pragma once

#include <GLEW/glew.h>
#include <string>
#include <memory>

class Shader
{
    GLuint _object;
public:
    static std::shared_ptr<Shader> shaderFromFile(const std::string& file, GLenum shaderType);
    
    Shader(const std::string& code, GLenum shaderType);
    Shader(Shader const&) = delete;
    ~Shader();
    
    void operator=(Shader const&) = delete;
    
    GLuint object() const;
};