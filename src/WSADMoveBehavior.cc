#include "WSADMoveBehavior.h"

#include "Entity.h"
#include "SpatialComponent.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>

using namespace Ymir;

void WSADMoveBehavior::update(double delta)
{
    //TODO use InputService (or something)
    float secondsElapsed = delta;
    auto spatial = entity->component<SpatialComponent>();
    
    const float moveSpeed = 2.0; //units per second
    if(glfwGetKey('S')){
        spatial->position += (secondsElapsed * moveSpeed * -spatial->forward());
    } else if(glfwGetKey('W')){
        spatial->position += (secondsElapsed * moveSpeed * spatial->forward());
    }
    if(glfwGetKey('A')){
        //spatial->position += (secondsElapsed * moveSpeed * -spatial->right());
        spatial->direction = glm::rotate(spatial->direction, secondsElapsed * 45, glm::vec3{0,1,0});
    } else if(glfwGetKey('D')){
        //spatial->position += (secondsElapsed * moveSpeed * spatial->right());
        spatial->direction = glm::rotate(spatial->direction, -secondsElapsed * 45, glm::vec3{0,1,0});
    }
    if(glfwGetKey('Z')){
        spatial->position += (secondsElapsed * moveSpeed * -glm::vec3(0,1,0));
    } else if(glfwGetKey('X')){
        spatial->position += (secondsElapsed * moveSpeed * glm::vec3(0,1,0));
    }
}