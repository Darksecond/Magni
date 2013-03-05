#pragma once

#include "Engine.h"
#include <OpenAL/alc.h>

namespace Ymir
{
    class AudioEngine : public Engine
    {
        ALCdevice* dev;
        ALCcontext* ctx;
    public:
        AudioEngine();
        ~AudioEngine();
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
    };
};