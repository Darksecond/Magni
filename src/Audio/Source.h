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
                playing = true;
                alSourcePlay(source);
            }
            
            void stop()
            {
                playing = false;
                alSourceStop(source);
            }
            
            void restart()
            {
                if(playing)
                {
                    stop();
                    play();
                }
            }
            
            bool isPlaying()
            {
                return playing;
            }
            
            void setPosition(glm::vec3 new_pos);
        };
    };
};