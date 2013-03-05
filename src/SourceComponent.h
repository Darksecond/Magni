#pragma once

#include "Component.h"
#include "Audio/Buffer.h"

#include <memory>

namespace Ymir
{
    class SourceComponent : public Component<SourceComponent>
    {
    public:
        SourceComponent(std::shared_ptr<Audio::Buffer> b) : buffer{b}, playing{false}
        {
        }
        
        std::shared_ptr<Audio::Buffer> buffer;
        bool playing;
    };
};