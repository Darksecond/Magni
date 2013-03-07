#pragma once

#include "Behavior.h"
#include "Renderer/RenderEngine.h"
#include "Renderer/Text.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class WSADMoveBehavior : public Behavior
    {
        std::shared_ptr<Text> text;
    public:
        WSADMoveBehavior(RenderEngine&);
        virtual void update(double delta);
    };
};
