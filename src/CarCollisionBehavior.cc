#include "CarCollisionBehavior.h"

#include "SpatialComponent.h"
#include "CarComponent.h"
#include "Entity.h"

#include <iostream>

using namespace Ymir;

CarCollisionBehavior::CarCollisionBehavior(std::shared_ptr<Audio::Buffer> buffer)
{
    audio_crash = std::unique_ptr<Audio::Source>{new Audio::Source{buffer}};
    audio_crash->noloop();
}

void CarCollisionBehavior::update(double delta)
{   
}

void CarCollisionBehavior::receive(message_type_t type, const Entity& trigger)
{
    if(type == message_type_t::COLLISION)
    {
        audio_crash->setPosition(entity->component<SpatialComponent>()->get_position());
        if(!audio_crash->isPlaying() && glm::abs(entity->component<CarComponent>()->gas) > 0.5) audio_crash->play();
        
        entity->component<CarComponent>()->gas = -0.1;
    }
}