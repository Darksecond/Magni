#pragma once

#include "Component.h"

#include <vector>
#include <memory>

namespace Ymir
{
    class Engine;
    class Entity;
    
    class EngineManager
    {
        std::vector<std::unique_ptr<Engine>> engines;
    public:
        void registerEntity(Entity& entity);
        void unregisterEntity(Entity& entity);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        void update(double delta);
        
        void assign(std::unique_ptr<Engine> engine);
        
        template<typename T, typename ... Args>
        void assign(Args && ... args);
    };
    
    //INLINE & TEMPLATE METHODS
    
    template<typename T, typename ... Args>
    void EngineManager::assign(Args && ... args)
    {
        assign(std::unique_ptr<T>{new T{args ...}});
    }
};