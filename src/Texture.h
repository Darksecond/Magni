#pragma once

#include <GLEW/glew.h>

#include "Bitmap.h"

class Texture
{
    GLuint _object;
    GLfloat _width;
    GLfloat _height;
public:
    Texture(Bitmap& bitmap, GLint minMagFiler = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
    Texture(Texture&& other);
    Texture(const Texture&) = delete;
    ~Texture();
    
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture&) = delete;
    
    GLuint object() const;
    GLfloat width() const;
    GLfloat height() const;
};