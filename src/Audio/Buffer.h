#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <ALUT/alut.h>

#include <string>

#include "StreamReader.h"

namespace Ymir
{
    namespace Audio
    {
        class Buffer
        {
            ALuint buffer;
        public:
            //TODO move to seperate loader
            static Buffer fromStream(StreamReader& stream)
            {
                ALsizei size;
                ALfloat freq;
                ALenum  format;
                ALvoid* data;
                
                char* input_data = new char[stream.size()];
                stream.read(input_data, stream.size());
                data = alutLoadMemoryFromFileImage(input_data, (ALsizei)stream.size(), &format, &size, &freq);
                
                ALenum err = alutGetError();
                if(err != ALUT_ERROR_NO_ERROR)
                {
                    std::string err_str(alutGetErrorString(err));
                    throw std::runtime_error("Could not load file " + err_str);
                }
                
                Buffer b{data, format, size, freq};
                
                free(data);
                delete [] input_data;
                
                return b;
            }
            
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