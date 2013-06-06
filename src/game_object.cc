#include "game_object.h"

game_object::game_object(const std::string& name, const glm::vec3& position) : _name(name), _children(), _spatial(position)
{
}

void game_object::add(std::shared_ptr<game_object> child)
{
    _children.push_back(child);
}

void game_object::accept(game_object_visitor &visitor)
{
    visitor.visit(*this);
    for(auto child : _children)
        child->accept(visitor);
}

const std::string& game_object::name() const
{
    return _name;
}