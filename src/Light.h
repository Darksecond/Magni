#pragma once

#include "Program.h"
#include "Camera.h"

#include <GLM/glm.hpp>

class Light
{
    glm::vec4 position;
    glm::vec3 attenuation;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
public:
    Light();
    explicit Light(glm::vec4 position);
    Light(glm::vec4 position, glm::vec3 color);
    Light(glm::vec4 position, glm::vec3 color, glm::vec3 attenuation);
    
    void attach(Program& p, const Camera& c) const;
    
    void setPosition(glm::vec4 position);
    void setAttenuation(glm::vec3 attenuation);
    void setDiffuse(glm::vec3 diffuse);
    void setAmbient(glm::vec3 ambient);
    void setSpecular(glm::vec3 specular);
};