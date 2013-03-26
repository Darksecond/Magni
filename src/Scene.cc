#include "Scene.h"
#include <iostream>
using namespace Ymir;

Scene::Scene(EngineManager& manager) : engines(manager)
{
}

Scene::~Scene()
{
    for(auto& entity : entities)
    {
        engines.unregisterEntity(*entity);
    }
}

Entity& Scene::assign(std::unique_ptr<Entity> entity, Entity* parent)
{
    Entity& retval = *entity;
    retval.parent = parent;
    entities.push_back(std::move(entity));
    engines.registerEntity(retval);
    return retval;
}

Entity& Scene::assign(const std::string& name, Entity* parent)
{
    std::unique_ptr<Entity> entity{new Entity{engines, name}};
    return assign(std::move(entity), parent);
}

void Scene::deleteEntity(Entity * entity) {
    std::cout<<"deleting: " << entity->name << std::endl;
    engines.unregisterEntity(*entity);
}
