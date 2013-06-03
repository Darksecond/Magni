#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class SizeComponent : public Component<SizeComponent>
    {
        public:
            SizeComponent(float offset, int xSize, int zSize );
            float offSetSelected;
            int x;
            int z;
    };
};
