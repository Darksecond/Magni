#include "FPSCameraBehavior.h"

#include "SpatialComponent.h"
#include "CameraComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw.h>

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

    _camera._horizontalAngle += rightAngle;
    while(_camera._horizontalAngle > 360.0f) _camera._horizontalAngle -= 360.0;
    while(_camera._horizontalAngle < 0.0f) _camera._horizontalAngle += 360.0;
    
    _camera._verticalAngle += upAngle;
    if(_camera._verticalAngle > MaxVerticalAngle) _camera._verticalAngle = MaxVerticalAngle;
    if(_camera._verticalAngle < -MaxVerticalAngle) _camera._verticalAngle = -MaxVerticalAngle;
    
    //we can't only use the quaternion, because it would be too difficult to check the maximum pitch right now
    glm::quat up = glm::angleAxis(_camera._verticalAngle, glm::vec3(1,0,0));
    glm::quat right = glm::angleAxis(_camera._horizontalAngle, glm::vec3(0,1,0));
    _spatial.direction = up * right;
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