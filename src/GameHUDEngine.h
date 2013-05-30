#pragma once

#include "HUDEngine.h"
#include "Engine.h"

#include <list>
#include <map>
#include <memory>
#include <string>

namespace Ymir
{
    class GameHUDItem
    {
        const char* texture;
        std::shared_ptr<HUDElement> element;
    public:
        GameHUDItem(const char* tex) : texture(tex), element(nullptr) {}
        
        friend class HUDGroup;
    };
    
    class HUDGroup
    {
        std::list<std::shared_ptr<GameHUDItem>> items;
    public:
        HUDGroup() : items() {}
        
        inline void addItem(std::shared_ptr<GameHUDItem> item)
        {
            items.push_back(item);
        }
        
        void activate(HUDEngine& hud_engine);
        void deactivate(HUDEngine& hud_engine);
        void update();
    };
    
    class GameHUDEngine : public Engine
    {
        HUDEngine& hud_engine;
        std::map<std::string, std::shared_ptr<HUDGroup>> groups;
        std::shared_ptr<HUDGroup> active_group;
    public:
        GameHUDEngine(HUDEngine& hud);
        
        virtual void registerEntity(Entity& ent);
        virtual void unregisterEntity(Entity& ent);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& ent, const BaseComponent::Type& component_type);
        
        std::shared_ptr<HUDGroup> addGroup(const std::string& name);
        void activateGroup(const std::string& name);
    };
};