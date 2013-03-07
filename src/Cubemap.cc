#include "Cubemap.h"

using namespace Ymir;

Cubemap::Cubemap(const std::vector<Bitmap>& bitmaps, GLint minMagFilter, GLint wrapMode)
{
    glGenTextures(1, &_object);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _object);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, minMagFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
    
    int i = 0;
    for(auto& bitmap : bitmaps)
    {
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
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
                     0,
                     format,
                     (GLsizei)bitmap.width(),
                     (GLsizei)bitmap.height(),
                     0,
                     format,
                     GL_UNSIGNED_BYTE,
                     bitmap.pixelBuffer()
                     );
        i++;
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap::Cubemap(Cubemap&& other) : _object{other._object}
{
    other._object = 0;
}

Cubemap::~Cubemap()
{
    if(_object)
        glDeleteTextures(1, &_object);
}

Cubemap& Cubemap::operator=(Cubemap&& other)
{
    if(_object)
        glDeleteTextures(1, &_object);
    
    _object = other._object;
    other._object = 0;
    
    return *this;
}

GLuint Cubemap::object() const
{
    return _object;
}

void Cubemap::bind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _object);
}

void Cubemap::unbind(const GLenum unit) const
{
    if(unit)
        glActiveTexture(unit);
}