#pragma once

#include "buffer.h"

#ifdef __APPLE
#include <OpenAL/al.h>
#include <openAL/alc.h>
#endif
#ifdef _WIN32
#include <AL/al.h>
#include <AL/alc.h>
#endif // _WIN32

#include <GLM/glm.hpp>
#include <memory>

namespace audio
{
    class source
    {
    public:
        source(std::shared_ptr<buffer> buff);
        source(const source&) = delete;
        source(source&&);
        ~source();
        source& operator=(const source&) = delete;
        source& operator=(source&&);
    
        void set_position(const glm::vec3& pos);
        ALuint object();
        
        inline void play()
        {
            alSourcePlay(_source);
        }
        
        inline void stop()
        {
            alSourceStop(_source);
        }
        
        inline void noloop()
        {
            alSourcei(_source, AL_LOOPING, AL_FALSE);
        }
        
        inline bool is_playing()
        {
            ALenum state;
            alGetSourcei(_source, AL_SOURCE_STATE, &state);
            return (state == AL_PLAYING);
        }
        
        inline bool is_not_playing()
        {
            return !is_playing();
        }
    private:
        ALuint _source;
        std::shared_ptr<buffer> _buffer;
    };
};