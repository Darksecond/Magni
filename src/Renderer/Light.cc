#include "Light.h"

using namespace Ymir;

std::unique_ptr<Light> Light::fromEntity(const Entity& entity)
{
    //if entity has both Light and Spatial, make a Light
    LightComponent* lightComponent = entity.component<LightComponent>();
    SpatialComponent* spatialComponent = entity.component<SpatialComponent>();
    if(lightComponent != nullptr && spatialComponent != nullptr)
    {
        return std::unique_ptr<Light>{new Light{*lightComponent, *spatialComponent}};
    }
    return nullptr;
}

bool Light::needsComponent(const BaseComponent::Type type)
{
    if(type == LightComponent::type() || type == SpatialComponent::type())
        return true;
    return false;
}

Light::Light(LightComponent& light, SpatialComponent& spatial) : _light(light), _spatial(spatial)
{
}

void Light::attach(Program& p, const Camera& c) const
{
    p.setUniform("light.attenuation", _light.attenuation);
    p.setUniform("light.diffuse", _light.diffuse);
    p.setUniform("light.ambient", _light.ambient);
    p.setUniform("light.specular", _light.specular);
    int w = _light.lightType == LightComponent::LightType::POINT;
    p.setUniform("light.position", c.viewMatrix() * glm::vec4{_spatial.get_position(), w});
}

bool Light::operator==(const Light& other) const
{
    return &_light == &other._light && &_spatial == &other._spatial; //not tested yet
}
