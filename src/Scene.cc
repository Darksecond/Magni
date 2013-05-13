#include "Scene.h"
#include <iostream>
#include <typeinfo>
#include <sstream>

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
    int newnumber = ung->getNewUniqueNumber();
    std::stringstream ss;
    ss << name;
    ss << newnumber;
    std::cout << ss.str() << std::endl;
    std::unique_ptr<Entity> entity{new Entity{engines, ss.str(), newnumber}};
    return assign(std::move(entity), parent);
}

Entity& Scene::assign(const std::string& name, const int id, Entity* parent)
{
    std::stringstream ss;
    ss << name;
    ss << id;
    std::cout << ss.str() << std::endl;
    std::unique_ptr<Entity> entity{new Entity{engines, ss.str(), id}};
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

Entity* Scene::getEntity(int id) {
    for(auto& entity : entities)
    {
        if(entity.second->id == id) {
            return entity.second.get();
        }
    }

    return nullptr;
}
