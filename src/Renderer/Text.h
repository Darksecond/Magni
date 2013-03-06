#pragma once

#include <GLM/glm.hpp>
#include <string>

namespace Ymir
{
    class Text
    {
    public:
        std::string text;
        glm::vec2 position;
        int size;
        
        Text(std::string text, glm::vec2 pos, int size) : text{text}, position{pos}, size{size}
        {
        }
    };
};