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
            MoveComponent(int s, int rSpeed, std::vector<Tile> * tiles );
            int speed;
            int rotationSpeed;
            std::vector<Tile> * tiles;
    };
};
