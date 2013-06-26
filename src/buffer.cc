#include "buffer.h"

using namespace audio;

buffer::buffer(ALvoid* data, ALenum format, ALsizei size, ALsizei freq)
{
    alGenBuffers(1, &_buffer);
    
    if(alGetError() != AL_NO_ERROR)
        throw std::runtime_error("Could not create buffer");
    
    alBufferData(_buffer, format, data, size, freq);
    
    if(alGetError() != AL_NO_ERROR)
        throw std::runtime_error("Could not buffer data");
}

buffer::~buffer()
{
    if(_buffer)
        alDeleteBuffers(1, &_buffer);
}

buffer::buffer(buffer&& other) : _buffer(other._buffer)
{
    other._buffer = 0;
}

buffer& buffer::operator=(audio::buffer&& other)
{
    if(_buffer)
        alDeleteBuffers(1, &_buffer);
    
    _buffer = other._buffer;
    other._buffer = 0;
    
    return *this;
}

ALuint buffer::object() const
{
    return _buffer;
}