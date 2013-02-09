#include "Bitmap.h"
#include "stb_image.h"

using namespace Ymir;

//PRIVATE HELPER FUNCTIONS
inline unsigned int GetPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Bitmap::Format format)
{
    return (row * width + col) * static_cast<unsigned int>(format);
}

Bitmap Bitmap::bitmapFromFile(const std::string& file)
{
    int width, height, channels;
    unsigned char* pixels = stbi_load(file.c_str(), &width, &height, &channels, 0);
    if(!pixels)
        throw std::runtime_error(stbi_failure_reason());
    
    Bitmap bmp(width, height, (Format)channels, pixels);
    stbi_image_free(pixels);
    return bmp;
}

Bitmap::Bitmap(unsigned int width,
       unsigned int height,
       Format format,
               const unsigned char* pixels) : _width(width), _height(height), _format(format)
{
    size_t newSize = _width * _height * static_cast<unsigned int>(_format);
    _pixels = new unsigned char[newSize];
    memcpy(_pixels, pixels, newSize);
}

Bitmap::~Bitmap()
{
    delete[] _pixels;
}

void Bitmap::flipVertically() {
    unsigned long rowSize = static_cast<unsigned int>(_format) * _width;
    unsigned char* rowBuffer = new unsigned char[rowSize];
    unsigned halfRows = _height / 2;
    
    for(unsigned rowIdx = 0; rowIdx < halfRows; ++rowIdx){
        unsigned char* row = _pixels + GetPixelOffset(0, rowIdx, _width, _height, _format);
        unsigned char* oppositeRow = _pixels + GetPixelOffset(0, _height - rowIdx - 1, _width, _height, _format);
        
        memcpy(rowBuffer, row, rowSize);
        memcpy(row, oppositeRow, rowSize);
        memcpy(oppositeRow, rowBuffer, rowSize);
    }
    
    delete[] rowBuffer;
}

Bitmap::Format Bitmap::format() const
{
    return _format;
}

unsigned char* Bitmap::pixelBuffer() const
{
    return _pixels;
}

unsigned int Bitmap::width() const
{
    return _width;
}

unsigned int Bitmap::height() const
{
    return _height;
}