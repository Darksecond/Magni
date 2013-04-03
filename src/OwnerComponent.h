#pragma once
#include "Component.h"

namespace Ymir
{
    class OwnerComponent : public Component<OwnerComponent>
    {

    public:
        int playerNumber;

        explicit OwnerComponent(int playernumber);
    };
};
