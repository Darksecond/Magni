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
    unsigned char* pixelBuffer() const;
    unsigned int width() const;
    unsigned int height() const;
    Format format() const;
private:
    Format _format;
    unsigned int _width;
    unsigned int _height;
    unsigned char* _pixels;
};