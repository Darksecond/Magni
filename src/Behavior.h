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
        
        void setEntity(Entity* e)
        {
            entity = e;
        }
        
        //receive
    };
};