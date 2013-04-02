#pragma once

#include "Component.h"

namespace Ymir {
    class CurrencyComponent : public Component<CurrencyComponent> {
    public:
        CurrencyComponent(int e) : currency(e) {
            
        }
        
        /**
         * User starts with 0 currency, increments based on time and gameplay.
         * Should not reach negative values.
         */
        int currency;
    };
};