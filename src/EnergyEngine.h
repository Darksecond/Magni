#pragma once

#include "Engine.h"
#include "Renderer/RenderEngine.h"
#include "EnergyComponent.h"

#include <list>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

namespace Ymir
{
    class EnergyEngine : public Engine
    {
        std::list<EnergyComponent*> energies;
        std::shared_ptr<Text> produced_text;
        std::shared_ptr<Text> consumed_text;
        std::shared_ptr<Text> total_text;
        std::shared_ptr<Text> energy_warning_text;
        
    private:
        int total, produced, consumed;
        
    public:
        EnergyEngine(RenderEngine& renderer);

        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        
        int getTotal();
    };
};
