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

#include <iostream>

fpscam_behaviour::fpscam_behaviour(game_object& go) : behaviour(go), _angles(M_PI, 0)
{
}

void fpscam_behaviour::update()
{
    //TODO redo all this, it just doesn't work (not only the mouse part, moreso the keys part)
    /*
    int xpos, ypos;
    glfwGetMousePos(&xpos, &ypos);
    glfwSetMousePos(0, 0);
    
    const float mouse_speed = 0.001;
    _angles.x -= xpos * mouse_speed;
    _angles.y -= ypos * mouse_speed;
    
    if(_angles.x < 0)
        _angles.x = M_PI * 2;
    if(_angles.x > M_PI * 2)
        _angles.x = 0;
    
    if(_angles.y >= M_PI / 2)
        _angles.y = M_PI / 2;
    if(_angles.y <= -M_PI / 2)
        _angles.y = -M_PI / 2;
    
    glm::vec3 lookat;
    lookat.x = sinf(_angles.x) * cosf(_angles.y);
    lookat.y = sinf(_angles.y);
    lookat.z = cosf(_angles.x) * cosf(_angles.y);
    
    _parent.local().look_at(lookat, glm::vec3(0, 1, 0));
    */
    
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