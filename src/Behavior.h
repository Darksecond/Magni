#pragma once

namespace Ymir
{
    class Entity;
    
    class Behavior
    {
    protected:
        Entity* entity;
    public:
        virtual void update(double delta) = 0;
        
        //TODO move to constructor possibly?
        //TODO or create some kind of 'start', 'init' or 'create' virtual abstract method
        void setEntity(Entity* e)
        {
            entity = e;
        }
        
        //TODO receive
    };
};