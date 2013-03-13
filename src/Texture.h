#pragma once

#include "Bitmap.h"

//TODO TEMPORARY
#include "StreamReader.h"
#include "stb_image.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
#endif // _WIN32

#include <stdexcept>

namespace Ymir
{
    class Texture
    {
        GLuint _object;
        GLfloat _width;
        GLfloat _height;
    public:
        //TODO TEMPORARY
        //TODO replace with some kind of textureloader perhaps?
        static Texture fromStream(StreamReader& stream);

        Texture(Bitmap& bitmap, GLint minMagFilter = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
        Texture(Texture&& other);
        Texture(const Texture&) = delete;
        ~Texture();

        Texture& operator=(Texture&& other);
        Texture& operator=(const Texture&) = delete;

        GLuint object() const;
        GLfloat width() const;
        GLfloat height() const;

        void bind(const GLenum unit = 0) const;
        void unbind(const GLenum unit = 0) const;
    };

    class TextureContext
    {
        const Texture& texture;
        const GLenum unit;
    public:
        TextureContext(const Texture& t, const GLenum u = 0): texture(t), unit(u)
        {
            texture.bind(unit);
        }

        ~TextureContext()
        {
            texture.unbind(unit);
        }
    };
};
