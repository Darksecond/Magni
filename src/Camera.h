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
    glm::mat4 matrix() const;
    
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;
    
    inline void setAspectRatio(const float aspectratio) { _aspectratio = aspectratio; }
    
    inline void setPosition(const glm::vec3 pos) { _position = pos; }
    inline void offsetPosition(const glm::vec3& offset) { _position += offset; }
    
    void offsetOrientation(float upAngle, float rightAngle);
};