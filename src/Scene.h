#pragma once

#include "Entity.h"

#include <map>
#include <list>
#include <memory>

namespace Ymir
{
    class Scene
    {
        //TODO maybe make this a map, so we can map to entity names.
        std::list<std::unique_ptr<Entity>> entities;
        //engineManager& engines;
    public:
        //Scene(EngineManager& manager);
        Entity& assign(std::unique_ptr<Entity> entity);
        Entity& assign();
        
        void update(double delta);
    };
};