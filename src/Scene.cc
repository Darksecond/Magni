#include "Scene.h"
#include <iostream>
#include <typeinfo>
#include <sstream>

#include "SpatialComponent.h"

using namespace Ymir;

Scene::Scene(EngineManager& manager) : engines(manager)
{
    ung = new UniqueNumberGenerator();
}

Scene::~Scene()
{
    for(auto& entity : entities)
    {
        engines.unregisterEntity(*entity.second);
    }
}

Entity& Scene::assign(std::shared_ptr<Entity> entity, Entity* parent)
{
    Entity& retval = *entity;
    retval.parent = parent;
    entities.insert(std::pair<int, std::shared_ptr<Entity>>(entity->id, entity));
    engines.registerEntity(retval);
    return retval;
}

Entity& Scene::assign(const std::string& name, Entity* parent)
{
    int newnumber = ung->getNewUniqueNumber();
    std::cout << "assigning entity: " << name << std::endl;
    std::shared_ptr<Entity> entity = std::make_shared<Entity>(engines, name, newnumber);
    return assign(entity, parent);
}

Entity& Scene::assign(const std::string& name, const int id, Entity* parent)
{
    std::cout << "assigning entity: " << name << " with id: " << id << std::endl;
    std::shared_ptr<Entity> entity{new Entity{engines, name, id}};
    return assign(entity, parent);
}

void Scene::deleteEntity(Entity * entity)
{
    engines.unregisterEntity(*entity);
    entities.erase(entity->id);
}

Entity* Scene::getEntity(int id) {
    for(auto& entity : entities)
    {
        if(entity.second->id == id) {
            return entity.second.get();
        }
    }

    return nullptr;
}

std::shared_ptr<Entity> Scene::getEntityAtPosition(glm::vec3 position)
{
    double distance = 2.5f;
    std::shared_ptr<Entity> theEntity = nullptr;

    for (auto& entitiesEntry : entities)
    {
        std::shared_ptr<Entity>& entity = entitiesEntry.second;
        auto test = entity->component<SpatialComponent>();
        if(test == nullptr) continue;
        double distanceBetween = glm::distance(test->get_position(), position);

        if(distanceBetween < 2.5f && distanceBetween < distance) {
            distance = distanceBetween;
            theEntity = entity;
        }
    }

    return theEntity;
}
        
