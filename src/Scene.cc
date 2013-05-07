#include "Scene.h"
#include <iostream>
#include <typeinfo>

using namespace Ymir;

Scene::Scene(EngineManager& manager) : engines(manager)
{
}

Scene::~Scene()
{
    for(auto& entity : entities)
    {
        engines.unregisterEntity(*entity.second);
    }
}

Entity& Scene::assign(std::unique_ptr<Entity> entity, Entity* parent)
{
    Entity& retval = *entity;
    retval.parent = parent;
    entities.insert(std::pair<std::string, std::unique_ptr<Entity>>(entity->name, std::move(entity)));
    engines.registerEntity(retval);
    return retval;
}

Entity& Scene::assign(const std::string& name, Entity* parent)
{
    std::unique_ptr<Entity> entity{new Entity{engines, name}};
    return assign(std::move(entity), parent);
}

void Scene::deleteEntity(Entity * entity)
{
    engines.unregisterEntity(*entity);
    entities.erase(entity->name);
}

bool Scene::containsEntity(std::string name)
{
    auto it = entities.find(name);
    if (it == entities.end()) {
        return false;
    }
    return true;
}
