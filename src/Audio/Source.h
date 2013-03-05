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
        public:
            Source(std::shared_ptr<Buffer> buffer);
            ~Source();
            
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
            
            void setPosition(glm::vec3 new_pos);
        };
    };
};