#pragma once

#include <OpenAL/al.h>
#include <openAL/alc.h>
#include <GLm/glm.hpp>
#include <memory>

#include "Buffer.h"

namespace Ymir
{
    namespace Audio
    {
        class Source
        {
            ALuint source;
            std::shared_ptr<Buffer> buffer;
            bool playing;
        public:
            Source(std::shared_ptr<Buffer> buffer);
            Source(Source&) = delete;
            Source(Source&& other);
            ~Source();
            
            Source& operator=(Source&) = delete;
            Source& operator=(Source&& other);
            
            ALuint object()
            {
                return source;
            }
            
            void play()
            {
                alSourcePlay(source);
            }
            
            void stop()
            {
                alSourceStop(source);
            }
            
            bool isPlaying()
            {
                ALenum state;
                alGetSourcei(source, AL_SOURCE_STATE, &state);
                return (state == AL_PLAYING);
            }
            
            void setPosition(glm::vec3 new_pos);
        };
    };
};