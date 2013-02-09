#pragma once

#include "Entity.h"
#include "CameraComponent.h"
#include "SpatialComponent.h"

#include <GLM/glm.hpp>

class Camera
{
    CameraComponent& _camera;
    SpatialComponent& _spatial;
public:
    static std::unique_ptr<Camera> fromEntity(const Entity& e);

    Camera(CameraComponent& c, SpatialComponent& s);
    
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