#pragma once

#include "Engine.h"
#include "Renderer/RenderEngine.h"
#include "Renderer/Image.h"
#include "Renderer/Text.h"
#include "Renderer/HUDElement.h"
#include "Scene.h"

#include <list>
#include <memory>

namespace Ymir
{
    class Entity;

    class HUDEngine : public Engine
    {
        RenderEngine& renderEngine;
        ResourceManager<Texture>& textureManager;
        std::list<std::shared_ptr<HUDElement>> elements;
        bool _selection_selecting;
        glm::vec3 _selection_start;
        glm::vec3 _selection_end;
        std::list<std::shared_ptr<Entity>> _selection_entities;
    public:
        Scene* scene;

        HUDEngine(RenderEngine& renderer, ResourceManager<Texture>& texMan);

        virtual void registerEntity(Entity& ent);
        virtual void unregisterEntity(Entity& ent);
        virtual void update(int pass, double delta);
        virtual void addComponent(Entity& ent, const BaseComponent::Type& component_type);

        std::shared_ptr<Image> addImage(const char* texture, glm::vec2 pos, float w, float h);
        std::shared_ptr<Text> addText(const char* text, glm::vec2 pos, int size = 20);

        void removeElement(std::shared_ptr<HUDElement> element);


        std::shared_ptr<Entity> selectedEntity();
        std::list<std::shared_ptr<Entity>> selectedEntities();
    };
};