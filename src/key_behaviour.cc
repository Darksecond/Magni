#include "key_behaviour.h"
#include "game_object.h"
#include "game.h"
#include "scene.h"

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
        
        //Because we only get a pointer, not a shared_ptr, we need to fetch it ourselves.
        auto object = game::instance().active_scene()->get_by_name(_parent->name());
        game::instance().active_scene()->remove_game_object(object);
    }
}