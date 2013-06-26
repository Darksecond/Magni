#include "door_behaviour.h"
#include "game_object.h"
#include "aabb.h"
#include "time.h"

#include <memory>
#include <iostream>
#include <GLM/ext.hpp>

door_behaviour::door_behaviour()
{
    _moving = false;
    _height = 0;
}

void door_behaviour::update()
{
    if(_moving)
    {
        _height += 1.0f * time::instance().delta();
        if(_height > 10) _moving = false;
        _parent->local().translation_vec4().y = _height;
    }
}

void door_behaviour::on_collision(game_object& other)
{
    if(other.name() == "camera")
    {
        key_event event;
        event.key = _key;
        other.on_event(event_t::behaviour_has_key, &event);
        if(event.has_key)
            _moving = true;
    }
}
