#pragma once

#include "Component.h"
#include "Behavior.h"

#include "EngineManager.h"

#include <map>
#include <vector>
#include <memory>

#include "Message.h"

namespace Ymir
{    
    class Entity
    {
        std::map<BaseComponent::Type, std::shared_ptr<BaseComponent>> components;
        std::vector<std::unique_ptr<Behavior>> behaviors;
        EngineManager& engines; //TODO singleton?
    public:
        Entity* parent;

        Entity(EngineManager& manager);
        
        template<typename T, typename ... Args>
        T& assign(Args && ... args);
        
        template<typename T>
        T& assign(std::shared_ptr<T> component);

        //be sure that T has Component as an interface
        template<typename T>
        T* component() const;
        
        //TODO move to CC file
        void update(double delta)
        {
            for(auto& behavior : behaviors)
            {
                behavior->update(delta);
            }
        }
        
        void receive(message_type_t type) const
        {
            for(auto& behavior : behaviors)
            {
                behavior->receive(type);
            }
        }
        
        //TODO move to CC file
        void assignBehavior(std::unique_ptr<Behavior> behavior)
        {
            behavior->setEntity(this);
            behaviors.push_back(std::move(behavior));
            //behavior->init() (or create, or start)
        }
      
        //POSSIBLE OPTION:
        
        //TODO behavior gets a reference to this entity, in order to get to attributes and components
        //template<typename T>
        //std::shared_ptr<T> assignBehavior(std::shared_ptr<T> behavior);
        
        //TODO
        //template<typename T, typename ... Args>
        //std::shared_ptr<T> assignBehavior(Args && ... args);
        
        //TODO at the very least components should have some option of letting behaviors know that they were updated
    };

    //INLINED & TEMPLATE METHODS
    template<typename T>
    T* Entity::component() const
    {
        auto c = components.find(T::type());
        if(c == components.end())
            return nullptr;
        else
            return std::static_pointer_cast<T>(c->second).get();
    }

    template<typename T>
    T& Entity::assign(std::shared_ptr<T> component)
    {
        if(parent)
            component->parent = parent->component<T>();
        components.insert({component->type(), component});
        engines.addComponent(*this, component->type());
        return *component;
    }

    template<typename T, typename ... Args>
    T& Entity::assign(Args && ... args)
    {

        std::shared_ptr<T> component = std::make_shared<T>(args ...);
        return assign(component);
    }
};