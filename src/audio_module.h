#pragma once

#include "module.h"
#include "listener.h"

#ifdef __APPLE__
#include <OpenAL/alc.h>
#endif
#ifdef _WIN32
#include <AL/alc.h>
#endif
#include <memory>

class audio_module : public module
{
    ALCdevice* _dev;
    ALCcontext* _ctx;
    std::shared_ptr<audio::listener> _listener;
public:
    virtual void boot();
    virtual void build();
    virtual bool step(std::shared_ptr<scene>& active_scene);
    virtual void shutdown();
    virtual void on_event(const event_t type, void* data);
};