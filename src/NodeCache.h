#pragma once

#include "Entity.h"

#include <vector>
#include <memory>

namespace Ymir
{
    template<typename T, typename Container = std::vector<std::unique_ptr<T>>>
    class NodeCache
    {
    public:
        typedef T value_type;
        typedef Container container_type;
        
        void registerEntity(Entity& entity)
        {
            std::unique_ptr<T> node = T::fromEntity(entity);
            if(node)
            {
                container.push_back(std::move(node));
            }
        }
        
        void unregisterEntity(Entity& entity)
        {
            std::unique_ptr<T> node = T::fromEntity(entity);
            if(node)
            {
                container.erase(std::remove_if(container.begin(), container.end(),
                                            [&](const std::unique_ptr<T>& i)
                                            {
                                                return *node == *i;
                                            })
                             , container.end());
            }
        }
        
        void addComponent(Entity& entity, const BaseComponent::Type& type)
        {
            if(T::needsComponent(type))
            {
                std::unique_ptr<T> node = T::fromEntity(entity);
                if(node)
                    container.push_back(std::move(node));
            }
        }
        
        //TODO removeComponent
        
        typename Container::reference       operator[](typename Container::size_type pos) { return container[pos]; }
        typename Container::const_reference operator[](typename Container::size_type pos) const { return container[pos]; }
        typename Container::reference       front() { return container.front(); }
        typename Container::const_reference front() const { return container.front(); }
        typename Container::iterator        begin() { return container.begin(); }
        typename Container::const_iterator        begin() const { return container.begin(); }
        typename Container::const_iterator        cbegin() const { return container.cbegin(); }
        typename Container::iterator        end() { return container.end(); }
        typename Container::const_iterator        end() const { return container.end(); }
        typename Container::const_iterator        cend() const { return container.cend(); }
        
    //private:
        container_type container;
    };
};