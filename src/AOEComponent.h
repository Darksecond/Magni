#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class AOEComponent : public Component<AOEComponent>
    {
        public:
            AOEComponent(int r );
            int radius;
    };
};
