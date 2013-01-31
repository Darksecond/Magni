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
    
    void bind(const GLenum unit = 0) const;
    void unbind(const GLenum unit = 0) const;
};

class TextureContext
{
    const Texture& texture;
    const GLenum unit;
public:
    TextureContext(const Texture& t, const GLenum u = 0): texture{t}, unit{u}
    {
        texture.bind(unit);
    }
    
    ~TextureContext()
    {
        texture.unbind(unit);
    }
};