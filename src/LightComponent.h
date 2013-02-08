#pragma once

#include "Component.h"

#include <GLM/glm.hpp>

class LightComponent : public iComponent<LightComponent>
{
public:
    enum class LightType
    {
        //SPOT, //TODO no support for spotlights yet, shaders don't yet support it.
        DIRECTIONAL,
        POINT,
    };
    
    glm::vec3 attenuation;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    LightType lightType;
    
    explicit LightComponent(glm::vec3 color);
    LightComponent(glm::vec3 color, glm::vec3 attenuation);
};