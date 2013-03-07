#pragma once

#ifdef __APPLE__
    #include <OpenAL/al.h>
#endif
#ifdef _WIN32
    #include <AL/al.h>
#endif // _WIN32

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
            void setOrientation(glm::vec3 forward, glm::vec3 up);
        };
    };
};
