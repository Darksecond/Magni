#include "Texture.h"
#include <iostream>

using namespace Ymir;

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

void Texture::bind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, _object);
}

void Texture::unbind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::fromStream(StreamReader& stream)
{
    //make bitmap
    stbi_io_callbacks callbacks;
    callbacks.eof = [](void* user) -> int
    {
        StreamReader* reader = reinterpret_cast<StreamReader*>(user);
        return reader->eof() ? 0 : 1;
    };
    callbacks.read = [](void* user, char* data, int size) -> int
    {
        StreamReader* reader = reinterpret_cast<StreamReader*>(user);
        return static_cast<int>(reader->read(data, size));
    };
    callbacks.skip = [](void* user, unsigned n) -> void
    {
        StreamReader* reader = reinterpret_cast<StreamReader*>(user);
        reader->skip(n);
    };

    int width, height, channels;
    unsigned char* pixels = stbi_load_from_callbacks(&callbacks, reinterpret_cast<void*>(&stream), &width, &height, &channels, 0);
    if(!pixels)
        throw std::runtime_error(stbi_failure_reason());

    Bitmap bmp(width, height, static_cast<Bitmap::Format>(channels), pixels);
    stbi_image_free(pixels);

    //make texture
    return Texture{bmp};
}
