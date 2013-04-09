#include "EnergyEngine.h"
#include "EnergyComponent.h"

#include <GLM/glm.hpp>

using namespace Ymir;

EnergyEngine::EnergyEngine(RenderEngine& renderer) : energies()
{
    //text!
    produced_text = std::make_shared<Text>("0 E produced", glm::vec2{10, 50}, 20);
    consumed_text = std::make_shared<Text>("0 E used", glm::vec2{10, 30}, 20);

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
        std::stringstream produced_text_stream;
        produced_text_stream << produced << " E produced";
        produced_text->text = produced_text_stream.str();
        std::stringstream consumed_text_stream;
        consumed_text_stream << consumed << " E used";
        consumed_text->text = consumed_text_stream.str();
    }
}

void EnergyEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    if(component_type == EnergyComponent::type())
    {
        energies.push_back(entity.component<EnergyComponent>());
    }
}
