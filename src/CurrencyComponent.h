#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class CurrencyComponent : public Component<CurrencyComponent>
    {
    public:
        CurrencyComponent(uint32_t p) : price(p) {}
        int32_t price;
    };
};
