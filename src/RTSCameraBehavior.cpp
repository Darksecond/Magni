#include "RTSCameraBehavior.h"

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
#include <iostream>

using namespace Ymir;

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

void RTSCameraBehavior::update(double delta)
{
    offsetOrientation(delta);
}

//horiz en vert angle naar behavior (als private)
void RTSCameraBehavior::offsetOrientation(double delta) {
    auto spatial = entity->component<SpatialComponent>();
    auto camera = entity->component<CameraComponent>();
    CameraComponent& _camera = *camera;
    SpatialComponent& _spatial = *spatial;

    const float mouseSensitivity  = 0.1;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);

    float secondsElapsed = delta;

    if(glfwGetKey('S') || mouseY > 580 ) {
        spatial->set_position(spatial->get_position() + (secondsElapsed * 2.0f * glm::vec3(0,0,1)));
    }

    if(glfwGetKey('A') || mouseX < 20) {
        spatial->set_position(spatial->get_position() + (secondsElapsed * 2.0f * glm::vec3(-1,0,0)));
    }

    if(glfwGetKey('D') || mouseX > 780) {
        spatial->set_position(spatial->get_position() + (secondsElapsed * 2.0f * glm::vec3(1,0,0)));
    }

    if(glfwGetKey('W') || mouseY < 20 ) {
        spatial->set_position(spatial->get_position() + (secondsElapsed * 2.0f * glm::vec3(0,0,-1)));
    }

    if(glfwGetMouseWheel()) {
        if(lastScrollWheelIndex == NULL)
            lastScrollWheelIndex = 0;

        int mouseWheelIndex     = glfwGetMouseWheel();
        int realWheelIndex      = lastScrollWheelIndex - mouseWheelIndex;
        glm::vec3 newPosition   = spatial->get_position() + spatial->forward(realWheelIndex);

        if(newPosition.y < 10 && newPosition.y > 2)
            spatial->set_position(spatial->get_position() + spatial->forward(realWheelIndex));

        lastScrollWheelIndex = mouseWheelIndex;
    }
}
