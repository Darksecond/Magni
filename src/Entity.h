#pragma once

#include <map>
#include <vector>
#include <memory>

#include "Component.h"

namespace Ymir
{
    class Entity
    {
        std::map<BaseComponent::Type, std::shared_ptr<BaseComponent>> components;
        //TODO std::vector<std::shared_ptr<Behavior>> behaviors;
    public:
        Entity();
        
        template<typename T, typename ... Args>
        std::shared_ptr<T> assign(Args && ... args);
        
        template<typename T>
        std::shared_ptr<T> assign(std::shared_ptr<T> component);

        //be sure that T has Component as an interface
        template<typename T>
        std::shared_ptr<T> component() const;
        
        template<typename T>
        std::unique_ptr<T> node() const;
      
        //POSSIBLE OPTION:
        //TODO
        //void update(pass, delta)
        //  for(auto& behavior : behaviors)
        //    behavior.update(pass, delta)
        
        //TODO behavior gets a reference to this entity, in order to get to attributes and components
        //template<typename T>
        //std::shared_ptr<T> assignBehavior(std::shared_ptr<T> behavior);
        
        //TODO
        //template<typename T, typename ... Args>
        //std::shared_ptr<T> assignBehavior(Args && ... args);
        
        //TODO basically what components are right now
        //TODO may not be necessary
        //<something> attribute(<identifier>)
        
        //TODO behavior class
        //TODO behavior has update(pass, delta)
        //TODO behavior has receive(<message>)
    };

    //INLINED & TEMPLATE METHODS
    template<typename T>
    std::unique_ptr<T> Entity::node() const
    {
        return T::fromEntity(*this);
    }

    template<typename T>
    std::shared_ptr<T> Entity::component() const
    {
        auto c = components.find(T::type());
        if(c == components.end())
            return std::shared_ptr<T>{};
        else
            return std::static_pointer_cast<T>(c->second);
    }

    template<typename T>
    std::shared_ptr<T> Entity::assign(std::shared_ptr<T> component)
    {
        components.insert({component->type(), component});
        return component;
    }

    template<typename T, typename ... Args>
    std::shared_ptr<T> Entity::assign(Args && ... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(args ...);
        components.insert({T::type(), component});
        return component;
    }
};