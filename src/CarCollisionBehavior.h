#pragma once

#include "Behavior.h"
#include "Audio/Source.h"
#include "Audio/Buffer.h"

namespace Ymir
{
    class CarCollisionBehavior : public Behavior
    {
        std::unique_ptr<Audio::Source> audio_crash;
    public:
        CarCollisionBehavior(std::shared_ptr<Audio::Buffer> buffer);
        virtual void update(double delta);
        virtual void receive(message_type_t type, const Entity& trigger);
    };
};