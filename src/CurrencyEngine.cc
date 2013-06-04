//
//  CurrencyEngine.cpp
//  Ymir
//
//  Created by Nick und Peter on 26-03-13.
//  Copyright (c) 2013 Tim Peters. All rights reserved.
//
#include "CurrencyEngine.h"
#include "CurrencyComponent.h"
#include "GLM/glm.hpp"
#include <iomanip>
using namespace Ymir;

CurrencyEngine::CurrencyEngine(RenderEngine& renderer) : currency(0) {
    currencyText = std::make_shared<Text>("You have 0 Money", glm::vec2{10, 10},20); //position: 300,50, font size 10
    renderer.addText(currencyText);
}

void CurrencyEngine::registerEntity(Entity& ent) {

}

void CurrencyEngine::unregisterEntity(Entity& ent) {

}

void CurrencyEngine::update(int pass, double delta) {
    if(pass == 0) {
        currency += delta * 6.2830 * 2;
        int temp = currency;
        std::stringstream produced_text_stream;
        produced_text_stream << std::setw(4) << temp << " Scraps";
        currencyText->text = produced_text_stream.str();
    }
}


void CurrencyEngine::addComponent(Entity& ent, const BaseComponent::Type& component_type) {

}
