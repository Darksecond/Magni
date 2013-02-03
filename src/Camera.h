#pragma once

#include <GLM/glm.hpp>

class Camera
{
    float _aspectratio;
    glm::vec3 _position;
    float _horizontalAngle;
    float _verticalAngle;
public:
    Camera();
    
    glm::mat4 orientation() const;
    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;
    
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;
    
    void setAspectRatio(const float aspectratio);
    void setPosition(const glm::vec3& pos);
    void offsetPosition(const glm::vec3& offset);
    
    void offsetOrientation(float upAngle, float rightAngle);
};