#include "Buffer.h"

#include <stdexcept>
#include <stdlib.h>
#include <string>

using namespace Ymir::Audio;

Buffer Buffer::fromFile(const std::string& filename)
{
    ALsizei size;
    ALfloat freq;
    ALenum  format;
    ALvoid* data;
    
    data = alutLoadMemoryFromFile(filename.c_str(), &format, &size, &freq);
    ALenum err = alutGetError();
    if(err != ALUT_ERROR_NO_ERROR)
    {
        std::string err_str(alutGetErrorString(err));
        throw std::runtime_error("Could not load file " + err_str);
    }

    Buffer b{data, format, size, freq};
    
    free(data);
    
    return b;
}

Buffer::Buffer(ALvoid* data, ALenum format, ALsizei size, ALsizei freq)
{
    alGenBuffers(1, &buffer);
    
    if(alGetError() != AL_NO_ERROR)
        throw std::runtime_error("Could not create buffer");
    
    alBufferData(buffer,format,data,size,freq);
    
    ALenum err = alGetError();
    if(err != AL_NO_ERROR)
        throw std::runtime_error("Could not buffer data");
}

Buffer::~Buffer()
{
    if(buffer)
        alDeleteBuffers(1, &buffer);
}

Buffer::Buffer(Buffer&& other) : buffer{other.buffer}
{
    other.buffer = 0;
}

Buffer& Buffer::operator=(Buffer&& other)
{
    if(buffer)
        alDeleteBuffers(1, &buffer);
    
    buffer = other.buffer;
    other.buffer = 0;
    
    return *this;
}

ALuint Buffer::object() const
{
    return buffer;
}