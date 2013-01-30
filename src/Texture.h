#pragma once

#include <GLEW/glew.h>

#include "Bitmap.h"

class Texture
{
    GLuint _object;
    GLfloat _width;
    GLfloat _height;
public:
    Texture(Bitmap& bitmap, GLint minMagFilter = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
    Texture(Texture&& other);
    Texture(const Texture&) = delete;
    ~Texture();
    
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture&) = delete;
    
    GLuint object() const;
    GLfloat width() const;
    GLfloat height() const;
    
    inline void bind() const;
    inline void unbind() const;
};

class TextureContext
{
    const Texture& texture;
public:
    TextureContext(const Texture& t, GLenum unit = 0): texture{t}
    {
        if(unit)
            glActiveTexture(unit);
        texture.bind();
    }
    
    ~TextureContext()
    {
        texture.unbind();
    }
};

//INLINED METHODS
inline void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _object);
}

inline void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}