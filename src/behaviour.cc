#include "behaviour.h"
#include "game_object.h"

void behaviour::notify(event_t type, void* data)
{
    _parent->notify(type, data);
}

void behaviour::set_parent(game_object* go)
{
    _parent = go;
}
