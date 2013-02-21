#include "Scene.h"

using namespace Ymir;

Entity& Scene::assign(std::unique_ptr<Entity> entity)
{
    Entity& retval = *entity;
    entities.push_back(std::move(entity));
    return retval;
}

Entity& Scene::assign()
{
    std::unique_ptr<Entity> entity{new Entity{}};
    return assign(std::move(entity));
}

void Scene::update(double delta)
{
    for(auto& entity : entities)
    {
        entity->update(delta);
    }
}