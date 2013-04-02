#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class OwnerComponent : public Component<OwnerComponent>
    {

    public:
        OwnerComponent(OwnerComponent&) = delete;
    };
};
