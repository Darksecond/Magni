#include "Camera.h"

#include "CameraComponent.h"
#include "SpatialComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

using namespace Ymir;

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

std::unique_ptr<Camera> Camera::fromEntity(const Entity& entity)
{
    //if entity has both Light and Spatial, make a Light
    CameraComponent* cameraComponent = entity.component<CameraComponent>();
    SpatialComponent* spatialComponent = entity.component<SpatialComponent>();
    if(cameraComponent != nullptr && spatialComponent != nullptr)
    {
        return std::unique_ptr<Camera>{new Camera{*cameraComponent, *spatialComponent}};
    }
    return nullptr;
}

Camera::Camera(CameraComponent& c, SpatialComponent& s) : _camera{c}, _spatial{s}
{
}

glm::mat4 Camera::orientation() const
{
    return _spatial.orientation();
    //return glm::mat4_cast(_spatial.direction);
}

glm::mat4 Camera::viewMatrix() const
{
    glm::mat4 camera;
    camera = glm::inverse(_spatial.orientation());
    ////camera = glm::translate(camera, -_spatial.position);
    //camera = glm::translate(camera, _spatial.parent->position + _spatial.position);
    camera = glm::translate(camera, _spatial.globalPosition());
    return camera;
}

glm::mat4 Camera::projectionMatrix() const
{
    //50.0 = field of view
    //0.01  = near plane
    //100.0 = far plane
    return glm::perspective<float>(50.0, _camera._aspectratio, 0.01, 100.0);
}

glm::vec3 Camera::forward() const {
    glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0,0,-1,0);
    return glm::vec3(forward);
}

glm::vec3 Camera::right() const {
    glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1,0,0,0);
    return glm::vec3(right);
}

glm::vec3 Camera::up() const {
    glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0,1,0,0);
    return glm::vec3(up);
}
