#pragma once

#include "Component.h"
#include "Texture.h"
#include "Mesh.h"

#include <memory>

namespace Ymir
{
    class ModelComponent : public iComponent<ModelComponent>
    {
    public:
        ModelComponent(std::shared_ptr<Mesh> m, std::shared_ptr<Texture> t) : mesh{m}, texture{t}
        {
        }
        
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Texture> texture;
    };
};