#include "fpscam_behaviour.h"

#include "game_object.h"
#include "time.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32
#include <GLM/glm.hpp>

fpscam_behaviour::fpscam_behaviour(game_object& go) : behaviour(go)
{
}

void fpscam_behaviour::update()
{
    if(glfwGetKey('W') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(0.0f, 0.0f, -time::instance().delta() * 5));
    else if(glfwGetKey('S') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(0.0f, 0.0f, time::instance().delta() * 5));
    else if(glfwGetKey('A') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(-time::instance().delta() * 5, 0.0f, 0.0f));
    else if(glfwGetKey('D') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(time::instance().delta() * 5, 0.0f, 0.0f));
    else if(glfwGetKey('X') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(0.0f, time::instance().delta() * 5, 0.0f));
    else if(glfwGetKey('Z') == GLFW_PRESS)
        _parent.local().translate(glm::vec3(0.0f, -time::instance().delta() * 5, 0.0f));
}