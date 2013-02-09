#pragma once

#include <GLEW/glew.h>
#include <string>
#include <memory>

namespace Ymir {
    class Shader
    {
        GLuint _object;
    public:
        static Shader shaderFromFile(const std::string& file, GLenum shaderType);
        
        Shader(const std::string& code, GLenum shaderType);
        Shader(Shader&& other);
        Shader(const Shader&) = delete;
        ~Shader();
        
        Shader& operator=(Shader&& other);
        Shader& operator=(const Shader&) = delete;
        
        GLuint object() const;
    };
};