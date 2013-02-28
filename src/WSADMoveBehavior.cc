#include "WSADMoveBehavior.h"

#include "Entity.h"
#include "SpatialComponent.h"
#include "CarComponent.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <GLM/gtx/vector_angle.hpp>

#include <iostream>

using namespace Ymir;

void WSADMoveBehavior::update(double delta)
{
    //TODO use InputService (or something)
    float secondsElapsed = delta;
    auto spatial = entity->component<SpatialComponent>();
    auto car = entity->component<CarComponent>();
    
    const float moveSpeed = 1.0; //units per second
    const float moveMax = 5.0;
    const float turnSpeed = 3.125;
    const float turnMax = 22.5;
    
    //std::cout << glm::length(v) << std::endl;
    
    if(glfwGetKey('S'))
    {
        //spatial->position += (secondsElapsed * 2.0f * -spatial->forward());
        
        if(car->gas > -moveMax)
            car->gas -= moveSpeed * secondsElapsed;
    }
    else if(glfwGetKey('W'))
    {
        //spatial->position += (secondsElapsed * 2.0f * spatial->forward());
        
        if(car->gas < moveMax)
            car->gas += moveSpeed * secondsElapsed;
    }
    else if(!glfwGetKey('W') && !glfwGetKey('S'))
    {
        //no turning keys were pushed
        if(car->gas > 0)
            car->gas -= moveSpeed * secondsElapsed;
        else if(car->gas < 0)
            car->gas += moveSpeed * secondsElapsed;
    }
    
    if(glfwGetKey('A'))
    {
        ////spatial->position += (secondsElapsed * moveSpeed * -spatial->right());
        //spatial->direction = glm::rotate(spatial->direction, secondsElapsed * 45, glm::vec3{0,1,0});
        
        if(car->steering > -turnMax)
            car->steering -= turnSpeed * secondsElapsed;
    }
    else if(glfwGetKey('D'))
    {
        ////spatial->position += (secondsElapsed * moveSpeed * spatial->right());
        //spatial->direction = glm::rotate(spatial->direction, -secondsElapsed * 45, glm::vec3{0,1,0});
        
        if(car->steering < turnMax)
            car->steering += turnSpeed * secondsElapsed;
    }
    else if(!glfwGetKey('A') && !glfwGetKey('D'))
    {
        //no turning keys were pushed
        if(car->steering > 0)
            car->steering -= turnSpeed * secondsElapsed;
        else if(car->steering < 0)
            car->steering += turnSpeed * secondsElapsed;
    }
    
    if(glfwGetKey('Z'))
    {
        spatial->position += (secondsElapsed * moveSpeed * -glm::vec3(0,1,0));
    }
    else if(glfwGetKey('X'))
    {
        spatial->position += (secondsElapsed * moveSpeed * glm::vec3(0,1,0));
    }
    
    spatial->position += car->gas * secondsElapsed * spatial->forward();
    if(car->gas != 0)
    {
        spatial->direction = glm::rotate(spatial->direction, -car->steering * glm::abs(car->gas)/10, glm::vec3{0,1,0});
    }
}