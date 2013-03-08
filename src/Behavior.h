#pragma once

#include "Message.h"

namespace Ymir
{
    class Entity;
    
    /**
     * A behavior works with components from one entity (and possibly it's parent).
     * The bulk of the game logic is contained within behaviors.
     */
    class Behavior
    {
    protected:
        /**
         * The entity this behavior belongs to.
         */
        Entity* entity;
    public:
        /**
         * This is called one a tick, delta is the time between calls in seconds.
         */
        virtual void update(double delta) = 0;
        
        /**
         * This is called when a message is received. 
         * This is mostly used for collisions and triggers right now.
         */
        virtual void receive(message_type_t type, const Entity& trigger)
        {
        }
        
        //TODO move to constructor possibly?
        //TODO or create some kind of 'start', 'init' or 'create' virtual abstract method
        void setEntity(Entity* e)
        {
            entity = e;
        }
        
        //TODO receive
    };
};