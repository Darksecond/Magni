#include "key_behaviour.h"
#include "game_object.h"

void key_behaviour::update()
{
}

void key_behaviour::on_collision(game_object& other)
{
    if(other.name() == "camera")
    {
        key_event event;
        event.key = _key;
        other.on_event(event_t::behaviour_add_key, &event);
        //TODO remove self from scene
    }
}