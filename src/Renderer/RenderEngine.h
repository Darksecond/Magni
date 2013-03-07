#pragma once

#include "Engine.h"

#include "Program.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"
#include "Text.h"
#include "NodeCache.h"
#include "ResourceManager.h"
#include "ProgramResourceLoader.h"
#include "CubemapResourceLoader.h"
#include "Cubemap.h"

#include <vector>
#include <list>

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
        std::shared_ptr<Program> sky_program; //skybox
        NodeCache<Light> lights;
        NodeCache<Model> models;
        std::shared_ptr<Texture> holstein;
        std::list<std::shared_ptr<Text>> texts;
        std::shared_ptr<Cubemap> sky;

        RenderEngine(ResourceManager<Program, ProgramResourceLoader>&,
                     ResourceManager<Texture>&,
                     ResourceManager<Cubemap, CubemapResourceLoader>&);
        ~RenderEngine();
        
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void update(int pass, double delta);
        void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        
        void addText(std::shared_ptr<Text> t)
        {
            texts.push_back(t);
        }
    };
};