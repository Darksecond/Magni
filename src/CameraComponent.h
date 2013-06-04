#pragma once

#include "Component.h"

namespace Ymir
{
    class CameraComponent : public Component<CameraComponent>
    {
        float _aspectratio;
        float _horizontalAngle;
        float _verticalAngle;
    public:
        explicit CameraComponent(float aspectRatio);
        
        inline void set_aspect_ratio(float ar) { _aspectratio = ar; }
        inline void set_horizontal_angle(float ha) { _horizontalAngle = ha; }
        inline void set_vertical_angle(float va) { _verticalAngle = va; }
        
        inline float get_aspect_ratio() { return _aspectratio; }
        inline float get_horizontal_angle() { return _horizontalAngle; }
        inline float get_vertical_angle() { return _verticalAngle; }
    };
};