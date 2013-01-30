#include "Texture.h"

Texture::Texture(Bitmap& bitmap, GLint minMagFilter, GLint wrapMode) : _width(bitmap.width()), _height(bitmap.height())
{
    glGenTextures(1, &_object);
    glBindTexture(GL_TEXTURE_2D, _object);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    GLenum format;
    switch(bitmap.format())
    {
        case Bitmap::Format::Grayscale:
            format = GL_LUMINANCE;
            break;
        case Bitmap::Format::GrayscaleAlpha:
            format = GL_LUMINANCE_ALPHA;
            break;
        case Bitmap::Format::RGB:
            format = GL_RGB;
            break;
        case Bitmap::Format::RGBA:
            format = GL_RGBA;
            break;
    }
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 format,
                 (GLsizei)bitmap.width(),
                 (GLsizei)bitmap.height(),
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 bitmap.pixelBuffer());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& other) : _object(other._object)
{
    other._object = 0;
}

Texture::~Texture()
{
    if(_object)
        glDeleteTextures(1, &_object);
}

Texture& Texture::operator=(Texture&& other)
{
    if(_object)
        glDeleteTextures(1, &_object);
    
    _object = other._object;
    other._object = 0;
    return *this;
}

GLuint Texture::object() const
{
    return _object;
}

GLfloat Texture::width() const
{
    return _width;
}

GLfloat Texture::height() const
{
    return _height;
}