#pragma once

#include "StreamReader.h"

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#ifdef _WIN32
#include <AL/al.h>
#include <AL/alc.h>
#endif // _WIN32

#include <ALUT/alut.h>

#include <string>
#include <stdexcept>
#include <cstdlib>

namespace audio
{
    class buffer
    {
    public:
        static buffer fromStream(Ymir::StreamReader& stream)
        {
            ALsizei size;
            ALfloat freq;
            ALenum  format;
            ALvoid* data;
            
            char* input_data = new char[stream.size()];
            stream.read(input_data, stream.size());
            data = alutLoadMemoryFromFileImage(input_data, (ALsizei)stream.size(), &format, &size, &freq);
            
            alGetError();
            ALenum err = alutGetError();
            if(err != ALUT_ERROR_NO_ERROR)
            {
                std::string err_str(alutGetErrorString(err));
                throw std::runtime_error("Could not load file " + err_str);
            }
            
            buffer b(data, format, size, freq);
            
            free(data);
            delete [] input_data;
            
            return b;
        }
        
        buffer(ALvoid* data, ALenum format, ALsizei size, ALsizei freq);
        buffer(const buffer&) = delete;
        buffer(buffer&& other);
        ~buffer();
        
        buffer& operator=(const buffer&) = delete;
        buffer& operator=(buffer&& other);
        
        ALuint object() const;
    private:
        ALuint _buffer;
    };
};