#pragma once

#include <GLEW/glew.h>

#include "Bitmap.h"

//TODO TEMPORARY
#include "StreamReader.h"
#include "stb_image.h"

class Texture
{
    GLuint _object;
    GLfloat _width;
    GLfloat _height;
public:
    //TODO TEMPORARY
    //TODO replace with some kind of textureloader perhaps?
    static Texture fromStream(StreamReader& stream)
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
        
        Bitmap bmp(width, height, (Bitmap::Format)channels, pixels);
        stbi_image_free(pixels);
        
        //make texture
        return Texture{bmp};
    }
    
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