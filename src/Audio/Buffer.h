#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <ALUT/alut.h>

#include <string>

namespace Ymir
{
    namespace Audio
    {
        class Buffer
        {
            ALuint buffer;
        public:
            static Buffer fromFile(const std::string& filename);
            
            Buffer(ALvoid* data, ALenum format, ALsizei size, ALsizei freq);
            Buffer(Buffer&) = delete;
            Buffer(Buffer&& other);
            ~Buffer();
            
            Buffer& operator=(Buffer&) = delete;
            Buffer& operator=(Buffer&& other);
            
            ALuint object() const;
        };
    };
};