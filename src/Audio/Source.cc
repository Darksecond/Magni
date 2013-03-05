#include "Source.h"

using namespace Ymir::Audio;

Source::Source(std::shared_ptr<Buffer> b) : buffer{b}
{
    alGenSources(1, &source);
    
    alSourcei(source, AL_BUFFER, buffer->object());
    
    //alSourcef(source, AL_PITCH, 1.0f);
    //alSourcef(source, AL_GAIN, 1.0f);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    
    setPosition(glm::vec3{0});
}

Source::~Source()
{
    if(source)
        alDeleteSources(1, &source);
}

void Source::setPosition(glm::vec3 new_pos)
{
    ALfloat source0Pos[]={ new_pos.x, new_pos.y, new_pos.z};
    alSourcefv(source, AL_POSITION, source0Pos);
}