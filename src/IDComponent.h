#pragma once

#include "Component.h"

namespace Ymir
{
    class IDComponent : public Component<IDComponent>
    {

    public:
        int uniqueID;
        explicit IDComponent(int id) : uniqueID(id) {}
    };
};
