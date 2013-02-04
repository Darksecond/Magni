#pragma once

#include "Component.h"

#include <GLM/glm.hpp>

class LightComponent : public iComponent<LightComponent>
{
public:
    glm::vec3 attenuation;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    bool spot; //0 is directional-, 1 is spotlight
    
    explicit LightComponent(glm::vec3 color);
    LightComponent(glm::vec3 color, glm::vec3 attenuation);
};