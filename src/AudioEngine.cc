#include "AudioEngine.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <ALUT/alut.h>
#include <stdexcept>
#include <string>

using namespace Ymir;

AudioEngine::AudioEngine() : listener{nullptr}, sources{}
{
    alutInitWithoutContext(nullptr, nullptr);
    dev = alcOpenDevice(nullptr);
    if(!dev)
        throw std::runtime_error("Could not open default device");
    
    ctx = alcCreateContext(dev, nullptr);
    alcMakeContextCurrent(ctx);
    if(!ctx)
        throw std::runtime_error("Could not create context");
}

AudioEngine::~AudioEngine()
{
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
    alutExit();
    delete listener;
}

void mapSource(SourceComponent& component, SpatialComponent& spatial, Audio::Source& source)
{
    source.setPosition(spatial.globalPosition());
    
    if(source.isPlaying() == true && component.playing == false)
    {
        source.stop();
    }
    if(source.isPlaying() == false && component.playing == true)
    {
        source.play();
    }
}

void mapListener(ListenerComponent& component, SpatialComponent& spatial, Audio::Listener& listener)
{
    listener.setPosition(spatial.globalPosition());
    listener.setOrientation(-spatial.forward(), spatial.up());
}

void AudioEngine::registerEntity(Entity& entity)
{
    //listener
    auto l = entity.component<ListenerComponent>();
    auto s = entity.component<SpatialComponent>();
    auto sc = entity.component<SourceComponent>();
    if(l != nullptr && s != nullptr)
    {
        listener = new listener_t{Audio::Listener{}, entity};
        mapListener(*l, *s, listener->listener);
    }
    
    //source
    if(s != nullptr && sc != nullptr)
    {
        source_t s{Audio::Source{sc->buffer}, entity};
        mapSource(*s.entity.component<SourceComponent>(), *s.entity.component<SpatialComponent>(), s.source);
        sources.push_back(std::move(s));
    }
}

void AudioEngine::unregisterEntity(Entity& entity)
{
    //TODO
}

void AudioEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    
    if(component_type == ListenerComponent::type() || component_type == SpatialComponent::type())
    {
        if(listener == nullptr)
        {
            auto l = entity.component<ListenerComponent>();
            auto s = entity.component<SpatialComponent>();
            if(l != nullptr && s != nullptr)
            {
                listener = new listener_t{Audio::Listener{}, entity};
                mapListener(*l, *s, listener->listener);

            }
        }
    }
    
    if(component_type == SpatialComponent::type() || component_type == SourceComponent::type())
    {
        auto sc = entity.component<SourceComponent>();
        auto sp = entity.component<SpatialComponent>();
        if(sc != nullptr && sp != nullptr)
        {
            source_t s{Audio::Source{sc->buffer}, entity};
            mapSource(*s.entity.component<SourceComponent>(), *s.entity.component<SpatialComponent>(), s.source);
            sources.push_back(std::move(s));
        }
    }
}

void AudioEngine::update(int pass, double delta)
{
    if(pass == 0)
    {
        if(listener != nullptr)
        {
            mapListener(*listener->entity.component<ListenerComponent>(), *listener->entity.component<SpatialComponent>(), listener->listener);
        }
        
        for(auto& s : sources)
        {
            mapSource(*s.entity.component<SourceComponent>(), *s.entity.component<SpatialComponent>(), s.source);
        }
    }
}