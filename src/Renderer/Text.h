#pragma once

#include <GLM/glm.hpp>
#include <string>

#include "HUDElement.h"

namespace Ymir
{
    class Text : public HUDElement
    {
    public:
        std::string text;
        int size;
        
        Text(std::string text, glm::vec2 pos, int size);
        
        virtual void accept(HUDElementVisitor& visitor);
    };
};