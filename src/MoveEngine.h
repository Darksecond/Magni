#pragma once

#include "Engine.h"
#include "Scene.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Entity.h"
#include <vector>

namespace Ymir
{
    class MoveEngine : public Engine
    {
        Scene* scene;
    public:
        std::vector<Entity*> entities;


        MoveEngine();

        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
    };
};
