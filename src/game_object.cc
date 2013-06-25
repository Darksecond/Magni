#include "game_object.h"

game_object::game_object(const std::string& name, const glm::vec3& position) : _name(name), _children(), _local_spatial(position), _global_spatial(), _behaviour(nullptr), _bounding_volume(nullptr), _listeners()
{
}

void game_object::add(std::shared_ptr<game_object> child)
{
    _children.push_back(child);
}

void game_object::accept(game_object_visitor &visitor)
{
    visitor.start_visit(*this);
    for(auto child : _children)
        child->accept(visitor);
    visitor.end_visit(*this);
}

const std::string& game_object::name() const
{
    return _name;
}

spatial& game_object::local()
{
    return _local_spatial;
}

spatial& game_object::global()
{
    return _global_spatial;
}

std::shared_ptr<bounding_volume> game_object::collider()
{
    return _bounding_volume;
}

behaviour* game_object::get_behaviour()
{
    return _behaviour.get();
}

void game_object::set_behaviour(std::unique_ptr<behaviour> b)
{
    _behaviour = std::move(b);
    _behaviour->set_parent(this);
}

void game_object::set_collider(std::shared_ptr<bounding_volume> bv)
{
    _bounding_volume = bv;
}

void game_object::update()
{
    if(_behaviour)
        _behaviour->update();
    
    for(auto child : _children)
        child->update();
}

void game_object::update_global(const spatial& new_global)
{
    _global_spatial = new_global;
    if(_bounding_volume)
        _bounding_volume->update_global(new_global);
}

void game_object::notify(event_t type, void* data)
{
    for(auto listener : _listeners)
    {
        listener->on_event(type, data);
    }
}

void game_object::add_listener(module* m)
{
    _listeners.push_back(m);
}
