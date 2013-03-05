#include "Listener.h"

#include <iostream>

using namespace Ymir::Audio;

Listener::Listener()
{
    //ALfloat listenerVel[]={0.0,0.0,0.0};
    ALfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
    
    //alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);
    setPosition(glm::vec3{0.0, 0.0, 0.0});
}

void Listener::setPosition(glm::vec3 new_pos)
{
    std::cout << "l " << new_pos.x << " " << new_pos.y << " " << new_pos.z << std::endl;
    ALfloat listenerPos[]={new_pos.x,new_pos.y,new_pos.z};
    alListenerfv(AL_POSITION,listenerPos);
}