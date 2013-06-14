#include "light.h"

light::light(const std::string& name, const glm::vec3& position, float radius) : game_object(name, position), _radius(radius)
{
}

void light::accept(game_object_visitor& v)
{
    v.start_visit(*this);
    for(auto child : _children)
        child->accept(v);
    v.end_visit(*this);
}

float light::radius()
{
    return _radius;
}
