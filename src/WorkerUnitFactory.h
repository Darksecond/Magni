#pragma once

#include "UnitFactory.h"
#include "Scene.h"
#include "Gameplay.h"

namespace Ymir
{
    class WorkerUnitFactory : public UnitFactory
    {
        Gameplay& gameplay;
    public:
        WorkerUnitFactory(Gameplay& g) : gameplay(g)
        {
        }
        
        virtual Entity& makeLocal(glm::vec3 position);
        virtual Entity& makeRemote(glm::vec3 position, int id);
    };
};