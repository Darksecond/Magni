#pragma once

#include "Component.h"

namespace Ymir
{
    class CameraComponent : public Component<CameraComponent>
    {
    public:
        float _aspectratio;
        float _horizontalAngle;
        float _verticalAngle;
        
        explicit CameraComponent(float aspectRatio);
    };
};