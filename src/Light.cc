#include "Light.h"

Light::Light() : attenuation{1.0, 0.0, 0.0},
    diffuse{1.0, 1.0, 1.0}, ambient{1.0, 1.0, 1.0}, specular{1.0, 1.0, 1.0},
    position{0.0, 0.0, 0.0, 0.0}
{
}

Light::Light(glm::vec4 pos) : attenuation{1.0, 0.0, 0.0},
    diffuse{1.0, 1.0, 1.0}, ambient{1.0, 1.0, 1.0}, specular{1.0, 1.0, 1.0},
    position{pos}
{
}

Light::Light(glm::vec4 pos, glm::vec3 color) : attenuation{1.0, 0.0, 0.0},
    diffuse{color}, ambient{color}, specular{color},
    position{pos}
{
}

Light::Light(glm::vec4 pos, glm::vec3 color, glm::vec3 att) : attenuation{att},
    diffuse{color}, ambient{color}, specular{color},
    position{pos}
{
}

void Light::attach(Program& p, const Camera& c) const
{
    p.setUniform("light.attenuation", attenuation);
    p.setUniform("light.diffuse", diffuse);
    p.setUniform("light.ambient", ambient);
    p.setUniform("light.specular", specular);
    p.setUniform("light.position", c.viewMatrix() * position);
}

void Light::setPosition(glm::vec4 newPosition)
{
    position = newPosition;
}

void Light::setAttenuation(glm::vec3 newAttenuation)
{
    attenuation = newAttenuation;
}

void Light::setDiffuse(glm::vec3 newDiffuse)
{
    diffuse = newDiffuse;
}

void Light::setAmbient(glm::vec3 newAmbient)
{
    ambient = newAmbient;
}

void Light::setSpecular(glm::vec3 newSpecular)
{
    specular = newSpecular;
}