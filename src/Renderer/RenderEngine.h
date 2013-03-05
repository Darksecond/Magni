#pragma once

#include "Engine.h"

#include "Program.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"
#include "NodeCache.h"
#include "ResourceManager.h"
#include "ProgramResourceLoader.h"

#include <vector>

namespace Ymir
{
    class RenderEngine : public Engine
    {
        void initGLFW();
        void initGLEW();
        void initOpenGL();
    public:
        std::unique_ptr<Camera> _camera;
        std::shared_ptr<Program> texture_program; //texture
        std::shared_ptr<Program> phong_program; //lights
        std::shared_ptr<Program> overlay_program; //overlay
        NodeCache<Light> lights;
        NodeCache<Model> models;

        RenderEngine(ResourceManager<Program, ProgramResourceLoader>&);
        ~RenderEngine();
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void update(int pass, double delta);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
    };
};