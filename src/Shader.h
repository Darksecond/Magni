#pragma once

#ifdef __APPLE__
    #include <GLEW/glew.h>
#endif

#ifdef __WIN32
    #include <GL/glew.h>
#endif // __WIN32

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
