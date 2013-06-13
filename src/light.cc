#include "light.h"

light::light(const std::string& name, const glm::vec3& position) : game_object(name, position), _attenuation{0.0f, 0.0f, 1.0f}
{
}

void light::accept(game_object_visitor& v)
{
    v.start_visit(*this);
    for(auto child : _children)
        child->accept(v);
    v.end_visit(*this);
}

const light::attenuation_t& light::attenuation() const
{
    return _attenuation;
}

void light::set_attenuation(float constant, float linear, float exp)
{
    _attenuation.constant = constant;
    _attenuation.linear = linear;
    _attenuation.exp = exp;
}
