#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

Camera::Camera() : _aspectratio{4.0/3.0}, _position{0, 0, 0}, _horizontalAngle{0}, _verticalAngle{0}
{
}

glm::mat4 Camera::orientation() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, _verticalAngle, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, _horizontalAngle, glm::vec3(0,1,0));
    return orientation;
}

glm::mat4 Camera::viewMatrix() const
{
    glm::mat4 camera = orientation();
    camera = glm::translate(camera, -_position);
    return camera;
}

glm::mat4 Camera::projectionMatrix() const
{
    //50.0 = field of view
    //0.01  = near plane
    //100.0 = far plane
    return glm::perspective<float>(50.0, _aspectratio, 0.01, 100.0);
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
    _horizontalAngle += rightAngle;
    while(_horizontalAngle > 360.0f) _horizontalAngle -= 360.0;
    while(_horizontalAngle < 0.0f) _horizontalAngle += 360.0;
    
    _verticalAngle += upAngle;
    if(_verticalAngle > MaxVerticalAngle) _verticalAngle = MaxVerticalAngle;
    if(_verticalAngle < -MaxVerticalAngle) _verticalAngle = -MaxVerticalAngle;
}

void Camera::setAspectRatio(const float aspectratio)
{
    _aspectratio = aspectratio;
}

void Camera::setPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Camera::offsetPosition(const glm::vec3& offset)
{
    _position += offset;
}
