#include "fpscam_behaviour.h"

#include "game_object.h"
#include "time.h"
#include "text.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32
#include <GLM/ext.hpp>

#include <iostream>

fpscam_behaviour::fpscam_behaviour() : _angles(M_PI+M_PI/2, 0)
{
    keys[key_event::key_t::blue] = false;
    keys[key_event::key_t::red] = false;
}

void fpscam_behaviour::update()
{
    int xpos, ypos;
    glfwGetMousePos(&xpos, &ypos);
    glfwSetMousePos(0, 0);
    
    const float mouse_speed = 0.005;
    _angles.x -= xpos * mouse_speed;
    _angles.y -= ypos * mouse_speed;
    
    if(_angles.x < 0)
        _angles.x = M_PI * 2 - 0.00001; //floating point error otherwise
    if(_angles.x > M_PI * 2)
        _angles.x = 0;
    
    if(_angles.y >= M_PI / 2)
        _angles.y = M_PI / 2;
    if(_angles.y <= -M_PI / 2)
        _angles.y = -M_PI / 2;
    
    if(glfwGetKey('W') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(0.0f, 0.0f, -time::instance().delta() * 5));
    else if(glfwGetKey('S') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(0.0f, 0.0f, time::instance().delta() * 5));
    if(glfwGetKey('A') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(-time::instance().delta() * 5, 0.0f, 0.0f));
    else if(glfwGetKey('D') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(time::instance().delta() * 5, 0.0f, 0.0f));
    if(glfwGetKey('X') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(0.0f, time::instance().delta() * 5, 0.0f));
    else if(glfwGetKey('Z') == GLFW_PRESS)
        _parent->local().translate(glm::vec3(0.0f, -time::instance().delta() * 5, 0.0f));
    
    glm::vec3 pos = _parent->local().translation();
    pos.y = 1.0f;
    _parent->local().identity();
    _parent->local().translate(pos); //translation *should* go last, i think.
    _parent->local().rotate(glm::degrees(_angles.x), glm::vec3(0, 1, 0));
    _parent->local().rotate(glm::degrees(_angles.y), glm::vec3(1, 0, 0));
}

void fpscam_behaviour::on_event(event_t type, void* data)
{
    if(type == event_t::behaviour_add_key)
    {
        key_event* event = (key_event*)data;
        
        bool first_time = keys[event->key] == false;
        keys[event->key] = true;
        
        text_event t;
        if(event->key == key_event::key_t::blue && first_time)
            t._text = std::make_shared<::text>(glm::vec2(50, 50), "BLUE", 20);
        else if(event->key == key_event::key_t::red && first_time)
            t._text = std::make_shared<::text>(glm::vec2(150, 50), "RED", 20);
        else
            return;
        
        notify(event_t::renderer_add_text, &t);
    }
    else if(type == event_t::behaviour_has_key)
    {
        key_event* event = (key_event*)data;
        event->has_key = keys[event->key];
    }
}
