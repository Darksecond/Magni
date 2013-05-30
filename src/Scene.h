#pragma once

#include "Entity.h"

#include "EngineManager.h"
#include "UniqueNumberGenerator.h"

#include <map>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <GLM/glm.hpp>

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
        
        Entity& assign(std::shared_ptr<Entity> entity, Entity* parent = nullptr);
        Entity& assign(const std::string& name, Entity* parent = nullptr);
        Entity& assign(const std::string& name, const int id, Entity* parent = nullptr);
        void deleteEntity(Entity* entity);
        Entity* getEntity(int id);
        std::shared_ptr<Entity> getEntityAtPosition(glm::vec3 position);

        std::map<int, std::shared_ptr<Entity>> entities;
    };
};
