//
//  CurrencyEngine.cpp
//  Ymir
//
//  Created by Marvin Effing on 26-03-13.
//  Copyright (c) 2013 Tim Peters. All rights reserved.
//

#include "CurrencyEngine.h"
#include "CurrencyComponent.h"

#include "GLM/glm.hpp"
/*
using namespace Ymir;

CurrencyEngine::CurrencyEngine(RenderEngine& renderer) : currencylist() {

    positive_currency = std::make_shared<Text>("You have gained 0 currency total.", glm::vec2{300, 50},10); //position: 300,50, font size 10
    negative_currency = std::make_shared<Text>("You used 0 currency.",              glm::vec2{400, 60},10); //position: 400,50, font size 10

    renderer.addText(positive_currency);
    renderer.addText(negative_currency);
}

void CurrencyEngine::registerEntity(Entity& ent) {

}

void CurrencyEngine::unregisterEntity(Entity& ent) {

}

void CurrencyEngine::update(int pass, double delta) {
    if (pass == 0) {
        int positive = 0;
        int negative = 0;

        for (auto c : currencylist) {
            if (c->currency < 0)
                positive -= c->currency;
            if (c->currency > 0)
                negative += c->currency;
        }

        std::stringstream produced_text_stream;
        produced_text_stream << produced << " currency generated";
        produced_text->text = produced_text_stream.str();
        std::stringstream consumed_text_stream;
        consumed_text_stream << consumed << " currency spent";
        consumed_text->text = consumed_text_stream.str();
    }
}

void CurrencyEngine::addComponent(Entity& ent, const BaseComponent::Type& component_type) {
    if (component_type == CurrencyComponent::type()) {
        currencylist.push_back(ent.component<CurrencyComponent>());
    }
}
*/
