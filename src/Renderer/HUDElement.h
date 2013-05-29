#pragma once

#include <GLM/glm.hpp>

namespace Ymir
{
    class HUDElementVisitor;
    
    class HUDElement
    {
        bool _clicked = false;
    public:
        glm::vec2 bottom_left;
        glm::vec2 top_right;
        virtual void accept(HUDElementVisitor& visitor) = 0;
        
        bool clicked() { return _clicked; }
        void set_clicked(bool new_val) { _clicked = new_val; }
    };
};