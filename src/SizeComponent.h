#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class SizeComponent : public Component<SizeComponent>
    {
        public:
            SizeComponent(int xSize, int zSize );
            int x;
            int z;
    };
};
