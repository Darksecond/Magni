#include "audio_module.h"

#include <stdexcept>
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#ifdef _WIN32
#include <AL/al.h>
#include <AL/alc.h>
#endif // _WIN32
#include <ALUT/alut.h>

void audio_module::boot()
{
    alutInitWithoutContext(nullptr, nullptr);
    _dev = alcOpenDevice(nullptr);
    if(!_dev)
        throw std::runtime_error("Could not open default device");
    
    _ctx = alcCreateContext(_dev, nullptr);
    alcMakeContextCurrent(_ctx);
    if(!_ctx)
        throw std::runtime_error("Could not create context");
}

void audio_module::build()
{
    _listener = std::make_shared<audio::listener>();
}

bool audio_module::step(std::shared_ptr<scene>& active_scene)
{
    return true;
}

void audio_module::shutdown()
{
    alcDestroyContext(_ctx);
    alcCloseDevice(_dev);
    alutExit();
}

void audio_module::on_event(const event_t type, void* data)
{
    //TODO respond to 'play' message
}
