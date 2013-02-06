#pragma once

#include "Engine.h"

#include "Program.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Model.h"

#include <vector>

class RenderEngine : public Engine
{
    
public:
    std::unique_ptr<Camera> _camera;
    Program& _pt;
    Program& _pl;
    std::vector<std::unique_ptr<Light>> lights;
    std::vector<std::unique_ptr<Model>> models;


    RenderEngine(Program& pt, Program& pl);
    
    virtual void registerEntity(Entity& entity);
    virtual void unregisterEntity(Entity& entity);
    virtual void execute();
};