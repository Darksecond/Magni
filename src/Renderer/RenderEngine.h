#pragma once

#include "../Engine.h"

#include "../Program.h"
#include "../Texture.h"
#include "Camera.h"
#include "../Mesh.h"
#include "Light.h"
#include "Model.h"
#include "Text.h"
#include "../NodeCache.h"
#include "../ResourceManager.h"
#include "../ProgramResourceLoader.h"
#include "../CubemapResourceLoader.h"
#include "../Cubemap.h"
#include "../Tilemap.h"

#include <vector>
#include <list>

namespace Ymir
{
    class RenderEngine : public Engine
    {
        void initGLFW();
        void initGLEW();
        void initOpenGL();
        bool grid;
        TileMap* tileMap;
    public:
        std::unique_ptr<Camera> _camera;
        std::shared_ptr<Program> texture_program; //texture
        std::shared_ptr<Program> phong_program; //lights
        std::shared_ptr<Program> overlay_program; //overlay
        std::shared_ptr<Program> sky_program; //skybox

        // grid implementatie refactor nominatie
        std::shared_ptr<Program> grid_program;
        // einde refactor nominatie

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

        glm::vec3 GetTilePosition();
        glm::vec3 GetTilePositionFromCoordinates(int xPos, int zPos);
        glm::vec3 get3DPositionFromMousePosition();
        glm::vec3 get3DPositionFromCoordinates(int xPos, int yPos);

        // grid implementatie refactor nominatie
        void drawGrid(Program&, Camera&);
        void setGrid(bool);
        void setTileMap(TileMap*);
        // einde refactor nominatie

        void addText(std::shared_ptr<Text> t)
        {
            texts.push_back(t);
        }
    };
};
