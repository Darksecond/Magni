#include "Camera.h"

#include "CameraComponent.h"
#include "SpatialComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

std::unique_ptr<Camera> Camera::fromEntity(const Entity& entity)
{
    //if entity has both Light and Spatial, make a Light
    std::shared_ptr<CameraComponent> cameraComponent = entity.component<CameraComponent>();
    std::shared_ptr<SpatialComponent> spatialComponent = entity.component<SpatialComponent>();
    if(cameraComponent != nullptr && spatialComponent != nullptr)
    {
        return std::unique_ptr<Camera>{new Camera{cameraComponent, spatialComponent}};
    }
    return std::unique_ptr<Camera>{};
}

Camera::Camera(std::shared_ptr<CameraComponent> c, std::shared_ptr<SpatialComponent> s) : _camera{c}, _spatial{s}
{
}

glm::mat4 Camera::orientation() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, _spatial->direction.x, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, _spatial->direction.y, glm::vec3(0,1,0));
    return orientation;
}

glm::mat4 Camera::viewMatrix() const
{
    glm::mat4 camera = orientation();
    camera = glm::translate(camera, -_spatial->position);
    return camera;
}

glm::mat4 Camera::projectionMatrix() const
{
    //50.0 = field of view
    //0.01  = near plane
    //100.0 = far plane
    return glm::perspective<float>(50.0, _camera->_aspectratio, 0.01, 100.0);
}

glm::vec3 Camera::forward() const {
    glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0,0,-1,1);
    return glm::vec3(forward);
}

glm::vec3 Camera::right() const {
    glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

glm::vec3 Camera::up() const {
    glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}

void Camera::offsetOrientation(float upAngle, float rightAngle) {
    _camera->_horizontalAngle += rightAngle;
    while(_camera->_horizontalAngle > 360.0f) _camera->_horizontalAngle -= 360.0;
    while(_camera->_horizontalAngle < 0.0f) _camera->_horizontalAngle += 360.0;
    
    _camera->_verticalAngle += upAngle;
    if(_camera->_verticalAngle > MaxVerticalAngle) _camera->_verticalAngle = MaxVerticalAngle;
    if(_camera->_verticalAngle < -MaxVerticalAngle) _camera->_verticalAngle = -MaxVerticalAngle;
    
  
    //yeah, yeah, not a real direction vector...
    //...i will fix it eventually!
    _spatial->direction.x = _camera->_verticalAngle;
    _spatial->direction.y = _camera->_horizontalAngle;
}

void Camera::offsetPosition(const glm::vec3& offset)
{
    _spatial->position += offset;
}
