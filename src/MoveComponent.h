#pragma once

#include "Component.h"
#include "Program.h"
#include "Tile.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class MoveComponent : public Component<MoveComponent>
    {
        public:
            MoveComponent(float s, float rSpeed, std::vector<Tile> * tiles );
            float speed;
            float rotationSpeed;
            std::vector<Tile> * tiles;
    };
};
