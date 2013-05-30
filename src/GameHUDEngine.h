#pragma once

#include "HUDEngine.h"
#include "Engine.h"
#include "UnitFactory.h"

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
        std::shared_ptr<UnitFactory> unit_factory;
    public:
        GameHUDItem(const char* tex, std::shared_ptr<UnitFactory> fac) : texture(tex), element(nullptr), unit_factory(fac) {}
        
        friend class HUDGroup;
    };
    
    class HUDGroup
    {
        std::list<std::shared_ptr<GameHUDItem>> items;
    public:
        HUDGroup() : items() {}
        
        inline void addItem(const char* tex, std::shared_ptr<UnitFactory> fac)
        {
            items.push_back(std::make_shared<GameHUDItem>(tex, fac));
        }
        
        void activate(HUDEngine& hud_engine);
        void deactivate(HUDEngine& hud_engine);
        void update(HUDEngine&);
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