//
//  CurrencyEngine.h
//
//  Created by Nick Velthorst und Peter Heinen on 26-03-13.
//

#pragma once

#include <iostream>

#include "Engine.h"
#include "Renderer/RenderEngine.h"
#include "CurrencyComponent.h"

#include <list>
#include <memory>

namespace Ymir {
    class CurrencyEngine : public Engine {
        std::shared_ptr<Text> currencyText;

    public:
        CurrencyEngine(RenderEngine& renderer);

        double currency;

        virtual void registerEntity(Entity& ent);
        virtual void unregisterEntity(Entity& ent);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& ent, const BaseComponent::Type& component_type);
    };
};
