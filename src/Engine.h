#pragma once

#include "Component.h"

#include <memory>

namespace Ymir
{
    class Entity;
    
    /**
     * An engine uses the data from one or more components.
     * It is recommended to use an engine for performance sensitive tasks (like rendering).
     */
    class Engine
    {
    public:
        virtual void registerEntity(Entity& entity) = 0;
        virtual void unregisterEntity(Entity& entity) = 0;
        
        /**
         * This is called when a new component is added to an entity.
         * component_type is the type of component
         */
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type) = 0;
        
        /**
         * This is called every tick
         * there are multiple passes, see the main game loop for numbers.
         * you can use passes for ordering of tasks
         * delta is the time between loops in seconds
         */
        virtual void update(int pass, double delta) = 0;
    };
};