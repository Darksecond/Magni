#pragma once

#include <GLM/glm.hpp>

namespace Ymir
{
    class HUDElementVisitor;

    class HUDElement
    {
        bool _clicked;
        bool _mouse_up;
    public:
        HUDElement() : _clicked(false), _mouse_up(false) {}
        glm::vec2 bottom_left;
        glm::vec2 top_right;
        virtual void accept(HUDElementVisitor& visitor) = 0;

        bool clicked() { return _clicked; }
        void set_clicked(bool new_val)
        {
            if(new_val == false && _clicked == true)
                _mouse_up = true;
            else
                _mouse_up = false;

            _clicked = new_val;
        }

        bool mouse_up() { return _mouse_up; }
    };
};
