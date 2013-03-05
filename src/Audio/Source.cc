#include "Source.h"

#include <iostream>

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

Source::Source(Source&& other) : buffer{other.buffer}, source{other.source}
{
    other.buffer = nullptr;
    other.source = 0;
}

Source::~Source()
{
    if(source)
        alDeleteSources(1, &source);
}

Source& Source::operator=(Source&& other)
{
    if(source)
        alDeleteSources(1, &source);
    
    source = other.source;
    buffer = other.buffer;
    
    other.source = 0;
    other.buffer = nullptr;
    
    return *this;
}

void Source::setPosition(glm::vec3 new_pos)
{
    std::cout << "s " << new_pos.x << " " << new_pos.y << " " << new_pos.z << std::endl;
    ALfloat source0Pos[]={ new_pos.x, new_pos.y, new_pos.z};
    alSourcefv(source, AL_POSITION, source0Pos);
    //restart();
}