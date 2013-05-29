#pragma once

#include "Engine.h"
#include "RenderEngine.h"
#include "Image.h"
#include "Text.h"
#include "HUDElement.h"

#include <list>
#include <memory>

namespace Ymir
{
    class HUDEngine : public Engine
    {
        RenderEngine& renderEngine;
        ResourceManager<Texture>& textureManager;
        std::list<std::shared_ptr<HUDElement>> elements;
    public:
        HUDEngine(RenderEngine& renderer, ResourceManager<Texture>& texMan);
        
        virtual void registerEntity(Entity& ent);
        virtual void unregisterEntity(Entity& ent);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& ent, const BaseComponent::Type& component_type);
        
        std::shared_ptr<Image> addImage(const char* texture, glm::vec2 pos, float w, float h);
        std::shared_ptr<Text> addText(const char* text, glm::vec2 pos, int size = 20);
    };
};