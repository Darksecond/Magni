#pragma once

#include "events.h"

#include <memory>

class scene;
class module
{
public:
    virtual ~module() = default;
    
    virtual void boot() = 0;
    virtual void build() = 0;
    virtual bool step(std::shared_ptr<scene>& active_scene) = 0;
    virtual void shutdown() = 0;
    virtual void on_event(const event_t type, void* data) = 0;
};