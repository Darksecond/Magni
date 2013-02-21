#pragma once

#include "Engine.h"

#include "Program.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"
#include "NodeCache.h"

#include <vector>

namespace Ymir
{
    class RenderEngine : public Engine
    {
        
    public:
        std::unique_ptr<Camera> _camera;
        Program& _pt;
        Program& _pl;
        NodeCache<Light> lights;
        NodeCache<Model> models;

        RenderEngine(Program& pt, Program& pl);
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void update(double delta);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
    };
};