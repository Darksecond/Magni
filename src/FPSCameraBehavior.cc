#include "FPSCameraBehavior.h"

#include "SpatialComponent.h"
#include "CameraComponent.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/glfw.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32

using namespace Ymir;

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

void FPSCameraBehavior::update(double delta)
{
    const float mouseSensitivity = 0.1;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
    glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window
}

//horiz en vert angle naar behavior (als private)
void FPSCameraBehavior::offsetOrientation(float upAngle, float rightAngle) {
    auto spatial = entity->component<SpatialComponent>();
    auto camera = entity->component<CameraComponent>();
    CameraComponent& _camera = *camera;
    SpatialComponent& _spatial = *spatial;

    _camera.set_horizontal_angle(_camera.get_horizontal_angle() + rightAngle);
    while(_camera.get_horizontal_angle() > 360.0f) _camera.set_horizontal_angle(_camera.get_horizontal_angle() - 360.0);
    while(_camera.get_horizontal_angle() < 0.0f) _camera.set_horizontal_angle(_camera.get_horizontal_angle() + 360.0);

    _camera.set_vertical_angle(_camera.get_vertical_angle() + upAngle);
    if(_camera.get_vertical_angle() > MaxVerticalAngle) _camera.set_vertical_angle(MaxVerticalAngle);
    if(_camera.get_vertical_angle() < -MaxVerticalAngle) _camera.set_vertical_angle(-MaxVerticalAngle);

    //we can't only use the quaternion, because it would be too difficult to check the maximum pitch right now
    glm::quat up = glm::angleAxis(_camera.get_vertical_angle(), glm::vec3(1,0,0));
    glm::quat right = glm::angleAxis(_camera.get_horizontal_angle(), glm::vec3(0,1,0));
    _spatial.direction = glm::inverse(up * right);
}

/*
 //rotate camera based on mouse movement
 //TODO move this into a behavior for a camera
 const float mouseSensitivity = 0.1;
 int mouseX, mouseY;
 glfwGetMousePos(&mouseX, &mouseY);
 c.offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
 glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window



  //TODO move code to camerastrategy or something?
  //TODO or perhaps a script or behavior of some kind?
 //TODO move to behavior


void Camera::offsetPosition(const glm::vec3& offset)
{
    _spatial.position += offset;
}
*/
