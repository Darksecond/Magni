#pragma once

#include "Entity.h"

#include "EngineManager.h"

#include <map>
#include <list>
#include <memory>

namespace Ymir
{
    class Scene
    {
        //TODO maybe make this a map, so we can map to entity names.
        std::list<std::unique_ptr<Entity>> entities;
        EngineManager& engines;
    public:
        Scene(EngineManager& engines);
        ~Scene();
        Entity& assign(std::unique_ptr<Entity> entity, Entity* parent = nullptr);
        Entity& assign(Entity* parent = nullptr);
    };
};