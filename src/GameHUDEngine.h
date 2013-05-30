#pragma once

#include "HUDEngine.h"
#include "Engine.h"

#include <list>
#include <map>
#include <memory>
#include <string>

namespace Ymir
{
    class Gameplay;
    
    class GameHUDItem
    {
    public:
        typedef void (Gameplay::*builder)();
    private:
        const char* texture;
        std::shared_ptr<HUDElement> element;
        Gameplay& gameplay;
        builder _builder;
    public:
        
        GameHUDItem(const char* tex, Gameplay& g, builder b) : texture(tex), element(nullptr), gameplay(g), _builder(b) {}
        
        friend class HUDGroup;
    };
    
    class HUDGroup
    {
        std::list<std::shared_ptr<GameHUDItem>> items;
    public:
        HUDGroup() : items() {}
        
        inline void addItem(const char* tex, Gameplay& g, GameHUDItem::builder b)
        {
            items.push_back(std::make_shared<GameHUDItem>(tex, g, b));
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