#include "GameHUDEngine.h"

using namespace Ymir;

GameHUDEngine::GameHUDEngine(HUDEngine& hud) : hud_engine(hud), groups(), active_group(nullptr)
{
}

void GameHUDEngine::registerEntity(Entity& ent)
{
}

void GameHUDEngine::unregisterEntity(Entity& ent)
{
}

void GameHUDEngine::update(int pass, double delta)
{
    if(pass != 0) return;
    
    if(active_group)
        active_group->update(hud_engine);
}

void GameHUDEngine::addComponent(Entity& ent, const BaseComponent::Type& component_type)
{
}

std::shared_ptr<HUDGroup> GameHUDEngine::addGroup(const std::string& name)
{
    std::shared_ptr<HUDGroup> group = std::make_shared<HUDGroup>();
    groups.insert({name, group});
    
    return group;
}

void GameHUDEngine::activateGroup(const std::string& name)
{
    std::shared_ptr<HUDGroup> group = groups.at(name);
    
    if(active_group)
        active_group->deactivate(hud_engine);
    active_group = group;
    active_group->activate(hud_engine);
}

void HUDGroup::activate(HUDEngine& hud_engine)
{
    glm::vec2 pos(700.0f, 500.0f);
    
    for(auto i : items)
    {
        i->element = hud_engine.addImage(i->texture, pos, 100, 100);
        pos.y -= 100;
    }
}

void HUDGroup::deactivate(HUDEngine& hud_engine)
{
    for(auto i : items)
        if(i->element)
            hud_engine.removeElement(i->element);
}

void HUDGroup::update(HUDEngine& hud_engine)
{
    for(auto i : items)
    {
        if(i->element->mouse_up())
        {
            //TODO need a proper position
            glm::vec3 pos(0);
            if(hud_engine.selectedEntity())
                pos = hud_engine.selectedEntity()->component<SpatialComponent>()->get_position();
            i->unit_factory->makeLocal(pos);
        }
    }
}