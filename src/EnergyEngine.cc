#include "EnergyEngine.h"
#include "EnergyComponent.h"

#include <GLM/glm.hpp>

using namespace Ymir;

EnergyEngine::EnergyEngine(RenderEngine& renderer) : energies()
{
    //text!
    produced_text = std::make_shared<Text>("0 Energy produced", glm::vec2{500, 50}, 10);
    consumed_text = std::make_shared<Text>("0 Energy used", glm::vec2{500, 60}, 10);
    
    renderer.addText(produced_text);
    renderer.addText(consumed_text);
}

void EnergyEngine::registerEntity(Entity& entity)
{
}

void EnergyEngine::unregisterEntity(Entity& entity)
{
}

void EnergyEngine::update(int pass, double delta)
{
    if(pass == 0)
    {
        int produced = 0;
        int consumed = 0;
        
        for(auto c : energies)
        {
            if(c->energy_consumption < 0)
                produced -= c->energy_consumption; // -- makes plus
            if(c->energy_consumption > 0)
                consumed += c->energy_consumption;
        }
        
        //update texts
        produced_text->text = std::to_string(produced) + " Energy produced";
        consumed_text->text = std::to_string(consumed) + " Energy used";
    }
}

void EnergyEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    if(component_type == EnergyComponent::type())
    {
        energies.push_back(entity.component<EnergyComponent>());
    }
}