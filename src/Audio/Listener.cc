#include "Listener.h"

using namespace Ymir::Audio;

Listener::Listener()
{
    //ALfloat listenerVel[]={0.0,0.0,0.0};
    
    //alListenerfv(AL_VELOCITY,listenerVel);
    setPosition(glm::vec3{0.0, 0.0, 0.0});
    setOrientation(glm::vec3{0.0, 0.0, 1.0}, glm::vec3{0.0, 1.0, 0.0});
}

void Listener::setPosition(glm::vec3 new_pos)
{
    ALfloat listenerPos[]={new_pos.x,new_pos.y,new_pos.z};
    alListenerfv(AL_POSITION,listenerPos);
}

void Listener::setOrientation(glm::vec3 forward, glm::vec3 up)
{
    ALfloat listenerOri[]={forward.x,forward.y,forward.z, up.x,up.y,up.z};
    alListenerfv(AL_ORIENTATION,listenerOri);
}