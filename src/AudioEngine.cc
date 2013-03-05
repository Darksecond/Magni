#include "AudioEngine.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <ALUT/alut.h>
#include <stdexcept>
#include <string>

using namespace Ymir;

AudioEngine::AudioEngine()
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
}

void AudioEngine::registerEntity(Entity& entity)
{
}

void AudioEngine::unregisterEntity(Entity& entity)
{
}

void AudioEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
}

void AudioEngine::update(int pass, double delta)
{
}