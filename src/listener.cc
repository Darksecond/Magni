#include "listener.h"

using namespace audio;

listener::listener()
{
    setPosition(glm::vec3(0.0, 0.0, 0.0));
    setOrientation(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
}

void listener::setPosition(const glm::vec3& new_pos)
{
    ALfloat listenerPos[]={new_pos.x,new_pos.y,new_pos.z};
    alListenerfv(AL_POSITION,listenerPos);
}

void listener::setOrientation(const glm::vec3& forward, const glm::vec3& up)
{
    ALfloat listenerOri[]={forward.x,forward.y,forward.z, up.x,up.y,up.z};
    alListenerfv(AL_ORIENTATION,listenerOri);
}
