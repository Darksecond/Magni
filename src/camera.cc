#include "camera.h"

#include <GLM/ext.hpp>

camera::camera(const std::string& name, const float aspect_ratio, const glm::vec3& position) : game_object(name, position)
{
    _projection = glm::perspective(60.0f, aspect_ratio, 0.1f, 100.0f);
    
    glm::vec3 at(1.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    local().look_at(at, up);
}

void camera::accept(game_object_visitor& v)
{
    v.start_visit(*this);
    for(auto child : _children)
        child->accept(v);
    v.end_visit(*this);
}

const glm::mat4& camera::projection() const
{
    return _projection;
}
