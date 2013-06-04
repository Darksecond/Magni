#pragma once

#include "Component.h"

#include <vector>
#include <memory>
#include <map>
#include <typeinfo>

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
    public:
        std::map<size_t, std::unique_ptr<Engine>> engines;
        void registerEntity(Entity& entity);
        void unregisterEntity(Entity& entity);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        void update(int pass, double delta);

        template<typename T>
        T& assign(std::unique_ptr<T> engine);

        template<typename T, typename ... Args>
        T& assign(Args && ... args);
        
        template<typename T>
        T& get();
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
        engines[typeid(T).hash_code()] = std::move(engine);
        return ret;
    }

    template<typename T>
    T& EngineManager::get()
    {
        return *static_cast<T*>(engines.at(typeid(T).hash_code()).get());
    }
};
