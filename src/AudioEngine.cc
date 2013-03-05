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

void AudioEngine::registerEntity(Entity& entity)
{
    //listener
    auto l = entity.component<ListenerComponent>();
    auto s = entity.component<SpatialComponent>();
    auto sc = entity.component<SourceComponent>();
    if(l != nullptr && s != nullptr)
    {
        listener = new listener_t{Audio::Listener{}, entity};
        listener->listener.setPosition(listener->entity.component<SpatialComponent>()->globalPosition());
    }
    
    //source
    if(s != nullptr && sc != nullptr)
    {
        source_t s{Audio::Source{sc->buffer}, entity, false};
        s.source.setPosition(s.entity.component<SpatialComponent>()->globalPosition());
        if(s.playing == true && sc->playing == false)
        {
            s.playing = false;
            s.source.stop();
        }
        if(s.playing == false && sc->playing == true)
        {
            s.playing = true;
            s.source.play();
        }
        sources.push_back(std::move(s));
    }
}

void AudioEngine::unregisterEntity(Entity& entity)
{
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
                listener->listener.setPosition(listener->entity.component<SpatialComponent>()->globalPosition());
            }
        }
    }
    
    if(component_type == SpatialComponent::type() || component_type == SourceComponent::type())
    {
        auto sc = entity.component<SourceComponent>();
        auto sp = entity.component<SpatialComponent>();
        if(sc != nullptr && sp != nullptr)
        {
            source_t s{Audio::Source{sc->buffer}, entity, false};
            s.source.setPosition(s.entity.component<SpatialComponent>()->globalPosition());
            if(s.playing == true && sc->playing == false)
            {
                s.playing = false;
                s.source.stop();
            }
            if(s.playing == false && sc->playing == true)
            {
                s.playing = true;
                s.source.play();
            }
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
            glm::vec3 pos = listener->entity.component<SpatialComponent>()->globalPosition();
            listener->listener.setPosition(pos);
        }
        
        for(auto& s : sources)
        {
            s.source.setPosition(s.entity.component<SpatialComponent>()->globalPosition());
            auto sc = s.entity.component<SourceComponent>();
            if(s.playing == true && sc->playing == false)
            {
                s.playing = false;
                s.source.stop();
            }
            if(s.playing == false && sc->playing == true)
            {
                s.playing = true;
                s.source.play();
            }
        }
    }
}