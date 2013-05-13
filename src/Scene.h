#pragma once

#include "Entity.h"

#include "EngineManager.h"
#include "UniqueNumberGenerator.h"

#include <map>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace Ymir
{
    /**
     * A scene contains a number of entities.
     */
    class Scene
    {
        //TODO maybe make this a map, so we can map to entity names.
        EngineManager& engines;
    private:
        UniqueNumberGenerator* ung;

    public:
        Scene(EngineManager& engines);
        ~Scene();
        Entity& assign(std::unique_ptr<Entity> entity, Entity* parent = nullptr);
        Entity& assign(const std::string& name, Entity* parent = nullptr);
        Entity& assign(const std::string& name, const int id, Entity* parent = nullptr);
        void deleteEntity(Entity* entity);
        bool containsEntity(std::string name);
        Entity* getEntity(int id);

        std::map<std::string, std::unique_ptr<Entity>> entities;
    };
};
