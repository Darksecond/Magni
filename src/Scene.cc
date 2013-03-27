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
    std::list<std::unique_ptr<Entity>>::const_iterator iterator;
    std::list<std::unique_ptr<Entity>>::iterator it;

    std::cout << " LIJST VOOR : " << std::endl;
    for (it=entities.begin(); it != entities.end(); ++it) {
        std::cout << (*(*it).get()).name << std::endl;
    }

    for (it=entities.begin(); it != entities.end(); ++it) {
        if  (  (*(*it).get()).name  == entity->name ) {
            break;
        }
    }
   // std::cout << (*(*it).get()).name << std::endl;
    engines.unregisterEntity(*entity);
    entities.erase(it);

    std::cout << std::endl << " LIJST NA : " << std::endl;
    for (it=entities.begin(); it != entities.end(); ++it) {
        std::cout << (*(*it).get()).name << std::endl;
    }
}
