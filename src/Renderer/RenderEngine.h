#pragma once

#include "../Engine.h"

#include "../Program.h"
#include "../Texture.h"
#include "Camera.h"
#include "../Mesh.h"
#include "Light.h"
#include "Model.h"
#include "Text.h"
#include "Image.h"
#include "../NodeCache.h"
#include "../ResourceManager.h"
#include "../ProgramResourceLoader.h"
#include "../CubemapResourceLoader.h"
#include "../Cubemap.h"
#include "../Tilemap.h"

#include "HUDElement.h"
#include "../Laser.h"

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
    private:
        const static int bufferSize = 10 * 6; // amount of lasers we want to render * the 6 floats it takes to create a line
        const static int bufferSizeSelected = 10 * 4 * 6;// 6 points per line 4 lines per entity max 10 entities.
        GLfloat g_vertex_buffer_data[bufferSize];
        GLfloat g_vertex_buffer_dataSelected[bufferSizeSelected];
        int laserCount;
        int selectedCount;
    public:
        glm::vec3 laserBegin, laserEnd;

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
        std::list<std::shared_ptr<HUDElement>> texts;
        std::shared_ptr<Cubemap> sky;

        std::shared_ptr<Mesh> _square;
        std::shared_ptr<Texture> _grass;
        std::shared_ptr<Texture> _mountain;
        std::shared_ptr<Texture> _water;



        RenderEngine(ResourceManager<Program, ProgramResourceLoader>&,
                     ResourceManager<Texture>&,
                     ResourceManager<Cubemap, CubemapResourceLoader>&,
                     ResourceManager<Mesh>& meshManager
                     );
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
        void drawAOE (Program&, Camera&);
        void drawLaser(Program& p, Camera& c);
        void drawSelected(Program& p, Camera& c);
        void setGrid(bool);
        void setTileMap(TileMap*);
        void setLaserData(std::vector<Laser*> lasers);
        void setSelectedData(std::list<std::shared_ptr<Entity>> entities);
        // einde refactor nominatie

        void addText(std::shared_ptr<HUDElement> t)
        {
            texts.push_back(t);
        }
    };
};
