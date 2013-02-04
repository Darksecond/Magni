#pragma once

#include "Engine.h"

#include "Program.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

#include <vector>

class RenderEngine : public Engine
{
    std::vector<std::unique_ptr<Light>> lights;
    
    Program& _pt;
    Program& _pl;
    const Texture& _t;
    const Camera& _c;
    const Mesh& _m;
public:
    float gDegreesRotated;

    RenderEngine(Program& pt, Program& pl, const Texture& t, const Camera& c, const Mesh& m);
    
    virtual void registerEntity(Entity& entity);
    virtual void unregisterEntity(Entity& entity);
    virtual void execute();
};