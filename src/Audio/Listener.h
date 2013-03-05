#pragma once

#include <OpenAL/al.h>
#include <GLM/glm.hpp>

namespace Ymir
{
    namespace Audio
    {
        class Listener
        {
        public:
            Listener();
            void setPosition(glm::vec3 new_pos);
        };
    };
};