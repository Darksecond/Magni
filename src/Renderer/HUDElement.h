#pragma once

#include <GLM/glm.hpp>

namespace Ymir
{
    class HUDElementVisitor;
    
    class HUDElement
    {
    public:
        glm::vec2 bottom_left;
        glm::vec2 top_right;
        virtual void accept(HUDElementVisitor& visitor) = 0;
    };
};