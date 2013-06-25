#pragma once
#include <iostream>

#include "events.h"

class game_object;
class behaviour
{
public:
    behaviour() {}
    virtual ~behaviour() = default;
    
    virtual void update() = 0;
    virtual void on_collision(game_object& other) {};
    void notify(event_t type, void* data);
    void set_parent(game_object* go);
    
protected:
    game_object* _parent;
};