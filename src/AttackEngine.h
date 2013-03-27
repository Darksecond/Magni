#pragma once

#include "Engine.h"
#include "Scene.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Entity.h"

namespace Ymir
{
    class AttackEngine : public Engine
    {
        Scene* scene;
        ResourceManager<Mesh>& mesh_manager;
        ResourceManager<Texture>& texture_manager;
        
        Entity* _attack_animation_entity;
        double _attack_animation_life;
    public:
        AttackEngine(ResourceManager<Mesh>& meshManager, ResourceManager<Texture>& textureManager);
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
        
        void attack(Entity& attackee, const Entity& attacker);
        
        void setScene(Scene* s)
        {
            scene = s;
        }
    };
};