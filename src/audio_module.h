#pragma once

#include "module.h"

class audio_module : public module
{
public:
    virtual void boot();
    virtual void build();
    virtual bool step(std::shared_ptr<scene>& active_scene);
    virtual void shutdown();
    virtual void on_event(const event_t type, void* data);
};