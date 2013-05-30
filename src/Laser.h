#pragma once

#include "glm/glm.hpp"

namespace Ymir
{
    class Laser
    {
        private:

        public:
            Laser(glm::vec3 thisBeginPosition, glm::vec3 thisEndPosition, float time);
            glm::vec3 beginPosition, endPosition;
            float duration;
    };
};

