#include "source.h"

using namespace audio;

source::source(std::shared_ptr<buffer> buff) : _buffer(buff)
{
    alGenSources(1, &_source);
    alSourcei(_source, AL_BUFFER, _buffer->object());
    alSourcei(_source, AL_LOOPING, AL_TRUE);
    
    set_position(glm::vec3(0));
}

source::source(source&& other) : _source(other._source), _buffer(other._buffer)
{
    other._buffer = nullptr;
    other._source = 0;
}

source::~source()
{
    if(_source)
        alDeleteSources(1, &_source);
}

source& source::operator=(source&& other)
{
    if(_source)
        alDeleteSources(1, &_source);
    
    _source = other._source;
    _buffer = other._buffer;
    
    other._source = 0;
    other._buffer = nullptr;
    
    return *this;
}

void source::set_position(const glm::vec3& new_pos)
{
    ALfloat source0Pos[]={ new_pos.x, new_pos.y, new_pos.z};
    alSourcefv(_source, AL_POSITION, source0Pos);
}

ALuint source::object()
{
    return _source;
}