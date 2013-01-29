#pragma once

#include <string>

class Bitmap
{
public:
    enum class Format : unsigned int
    {
        Grayscale = 1,
        GrayscaleAlpha = 2,
        RGB = 3,
        RGBA = 4
    };

    static Bitmap bitmapFromFile(const std::string& file);
    
    Bitmap(unsigned int width,
            unsigned int height,
            Format format,
            const unsigned char* pixels);
    ~Bitmap();
    
    void flipVertically();
    inline unsigned char* pixelBuffer() const;
    inline unsigned int width() const;
    inline unsigned int height() const;
    inline Format format() const;
private:
    Format _format;
    unsigned int _width;
    unsigned int _height;
    unsigned char* _pixels;
};

//INLINED METHODS
inline Bitmap::Format Bitmap::format() const
{
    return _format;
}

inline unsigned char* Bitmap::pixelBuffer() const
{
    return _pixels;
}

inline unsigned int Bitmap::width() const
{
    return _width;
}

inline unsigned int Bitmap::height() const
{
    return _height;
}