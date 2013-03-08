#pragma once

#include "Component.h"

#include <vector>
#include <memory>

namespace Ymir
{
    class Engine;
    class Entity;
    
    /**
     * The EngineManager contains the engines that are used in the program.
     * It provides methods to update all engines and add new ones.
     */
    class EngineManager
    {
        std::vector<std::unique_ptr<Engine>> engines;
    public:
        void registerEntity(Entity& entity);
        void unregisterEntity(Entity& entity);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        void update(int pass, double delta);
        
        template<typename T>
        T& assign(std::unique_ptr<T> engine);
        
        template<typename T, typename ... Args>
        T& assign(Args && ... args);
    };
    
    //INLINE & TEMPLATE METHODS
    
    template<typename T, typename ... Args>
    T& EngineManager::assign(Args && ... args)
    {
        return assign(std::unique_ptr<T>{new T{args ...}});
    }
    
    template<typename T>
    T& EngineManager::assign(std::unique_ptr<T> engine)
    {
        T& ret = *engine;
        engines.push_back(std::move(engine));
        return ret;
    }
};