#pragma once
#include "Component.h"

namespace Ymir
{
    class RangeComponent : public Component<RangeComponent>
    {

    public:
        int range;

        explicit RangeComponent(int thisrange) : range(thisrange) {}
    };
};
