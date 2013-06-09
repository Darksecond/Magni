#include "game_object.h"

game_object::game_object(const std::string& name, const glm::vec3& position) : _name(name), _children(), _local_spatial(position), _global_spatial(), _behaviour(nullptr)
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

void game_object::set_behaviour(std::unique_ptr<behaviour> b)
{
    _behaviour = std::move(b);
}

void game_object::update()
{
    if(_behaviour)
        _behaviour->update();
    
    for(auto child : _children)
        child->update();
}