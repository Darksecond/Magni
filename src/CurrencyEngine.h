//
//  CurrencyEngine.h
//
//  Created by Marvin Effing on 26-03-13.
//

#pragma once

#include <iostream>

#include "Engine.h"
#include "RenderEngine.h"
#include "CurrencyComponent.h"

#include <list>
#include <memory>

namespace Ymir {
    class CurrencyEngine : public Engine {
        
        std::list<CurrencyComponent*> currencylist; //total current currency
        std::shared_ptr<Text> positive_currency; //your gained currency, gain currency over time and by achieving goals
        std::shared_ptr<Text> negative_currency; //your used currency
        
    public:
        CurrencyEngine(RenderEngine& renderer);
        
        virtual void registerEntity(Entity& ent);
        virtual void unregisterEntity(Entity& ent);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& ent, const BaseComponent::Type& component_type);
    };
};