#include "Scene.h"

using namespace Ymir;

Scene::Scene(EngineManager& manager) : engines{manager}
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

Entity& Scene::assign(Entity* parent)
{
    std::unique_ptr<Entity> entity{new Entity{engines}};
    return assign(std::move(entity), parent);
}