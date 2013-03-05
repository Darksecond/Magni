#pragma once

#include "Engine.h"
#include <OpenAL/alc.h>
#include "ListenerComponent.h"
#include "SpatialComponent.h"
#include "SourceComponent.h"
#include "Audio/Listener.h"
#include "Audio/Buffer.h"
#include "Audio/Source.h"
#include "Entity.h"

#include <list>

namespace Ymir
{
    class AudioEngine : public Engine
    {
        struct listener_t
        {
            Audio::Listener listener;
            Entity& entity;
        };
        
        struct source_t
        {
            Audio::Source source;
            Entity& entity;
        };
        
        ALCdevice* dev;
        ALCcontext* ctx;
        
        listener_t* listener;
        
        std::list<source_t> sources;
    public:
        AudioEngine();
        ~AudioEngine();
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
    };
};