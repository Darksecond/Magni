#pragma once

#include <memory>

#include "Entity.h"

class Engine
{
public:
    virtual void registerEntity(Entity& entity) = 0;
    virtual void unregisterEntity(Entity& entity) = 0;
    virtual void execute() = 0;
};