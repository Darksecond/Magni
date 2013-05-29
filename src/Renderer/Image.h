#pragma once

#include "HUDElement.h"
#include "Texture.h"

#include <memory>
#include <GLM/glm.hpp>

namespace Ymir
{
    class Image : public HUDElement
    {
    public:
        std::shared_ptr<Texture> image_texture;
        
        Image(glm::vec2 bottom_left, float width, float height, std::shared_ptr<Texture> tex);
        
        virtual void accept(HUDElementVisitor& visitor);
    };
};