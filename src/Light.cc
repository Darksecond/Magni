#include "Light.h"

Light::Light(std::shared_ptr<LightComponent> light, std::shared_ptr<SpatialComponent> spatial) : _light{light}, _spatial{spatial}
{
}

void Light::attach(Program& p, const Camera& c) const
{
    p.setUniform("light.attenuation", _light->attenuation);
    p.setUniform("light.diffuse", _light->diffuse);
    p.setUniform("light.ambient", _light->ambient);
    p.setUniform("light.specular", _light->specular);
    p.setUniform("light.position", c.viewMatrix() * glm::vec4{_spatial->position, _light->spot});
}