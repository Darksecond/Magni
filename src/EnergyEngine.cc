#include "EnergyEngine.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"

#include <GLM/glm.hpp>

using namespace Ymir;

EnergyEngine::EnergyEngine(RenderEngine& renderer) : energies()
{
    //text!
    total_text    = std::make_shared<Text>("0 Energy total", glm::vec2{500, 40},10);
    produced_text = std::make_shared<Text>("0 Energy produced", glm::vec2{500, 50}, 10);
    consumed_text = std::make_shared<Text>("0 Energy used", glm::vec2{500, 60}, 10);

    energy_warning_text = std::make_shared<Text>("", glm::vec2{10, 582}, 18);

    produced, consumed, total = 0;

    produced_text = std::make_shared<Text>("0 E produced", glm::vec2{10, 50}, 20);
    consumed_text = std::make_shared<Text>("0 E used", glm::vec2{10, 30}, 20);

    renderer.addText(produced_text);
    renderer.addText(consumed_text);
    renderer.addText(energy_warning_text);
}

void EnergyEngine::registerEntity(Entity& entity)
{
}

void EnergyEngine::unregisterEntity(Entity& entity)
{
    for(EnergyComponent* c : energies)
    {
        if(entity.component<EnergyComponent>() == c)
        {
            //delete c
            energies.remove(c);
            break;
        }
    }
}

void EnergyEngine::update(int pass, double delta)
{
    if(pass == 0)
    {
        produced = 0;
        consumed = 0;

        for(auto c : energies)
        {
            if(c->energy_consumption < 0) {
                produced -= c->energy_consumption; // -- makes plus
                total = (produced - consumed);
            }
            if(c->energy_consumption > 0) {
                consumed += c->energy_consumption;
                total = (produced - consumed);
            }
        }

        //update texts

        std::stringstream produced_text_stream;
        produced_text_stream << produced << " E produced";
        produced_text->text = produced_text_stream.str();

        std::stringstream consumed_text_stream;
        consumed_text_stream << consumed << " E used";
        consumed_text->text = consumed_text_stream.str();

        std::stringstream total_text_stream;
        total_text_stream << (produced - consumed) << " Energy total.";
        total_text->text = total_text_stream.str();

        if (total < 0) {
            //apply debuff to all units & buildings
            std::stringstream warning;
            warning << "Warning, low on energy. Debuff is applied.";
            energy_warning_text->text = warning.str();
            //gameplay.setDebuff();
        } else {
            std::stringstream clear;
            clear << "";
            energy_warning_text->text = clear.str();
        }

    }
}

void EnergyEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    if(component_type == EnergyComponent::type())
    {
        energies.push_back(entity.component<EnergyComponent>());
    }
}

int EnergyEngine::getTotal() {
    return total;
}

void EnergyEngine::setDebuff() {
    
}
