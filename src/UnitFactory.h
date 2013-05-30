#pragma once

#include "Entity.h"
#include <GLM/glm.hpp>

namespace Ymir
{
    class UnitFactory
    {
    public:
        virtual Entity& makeLocal(glm::vec3 position) = 0;
        virtual Entity& makeRemote(glm::vec3 position, int id) = 0;
    };
};