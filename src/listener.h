#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#endif
#ifdef _WIN32
#include <AL/al.h>
#endif // _WIN32

#include <GLM/glm.hpp>

namespace audio
{
    class listener
    {
    public:
        listener();
        void setPosition(const glm::vec3& pos);
        void setOrientation(const glm::vec3& forward, const glm::vec3& up);
    };
};