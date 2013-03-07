#pragma once

#include "Bitmap.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
#endif // _WIN32

#include <vector>
#include <memory>



namespace Ymir
{
    class Cubemap
    {
        GLuint _object;
    public:
        Cubemap(const std::vector<Bitmap>& bitmap, GLint minMagFilter = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
        Cubemap(Cubemap&& other);
        Cubemap(const Cubemap&) = delete;
        ~Cubemap();

        Cubemap& operator=(Cubemap&& other);
        Cubemap& operator=(const Cubemap&) = delete;

        GLuint object() const;

        void bind(const GLenum unit = 0) const;
        void unbind(const GLenum unit = 0) const;
    };
};
