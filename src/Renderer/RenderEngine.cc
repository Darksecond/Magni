#include "RenderEngine.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "Light.h"
#include "LightComponent.h"
#include "SpatialComponent.h"

using namespace Ymir;

RenderEngine::RenderEngine(Program& pt, Program& pl) : _pt{pt}, _pl{pl}, lights{}, models{}
{
}

void RenderEngine::registerEntity(Entity& entity)
{    
    lights.registerEntity(entity);
    models.registerEntity(entity);
    
    std::unique_ptr<Camera> cameraNode = Camera::fromEntity(entity);
    if(cameraNode)
    {
        _camera = std::move(cameraNode);
    }
}

void RenderEngine::unregisterEntity(Entity& entity)
{    
    lights.unregisterEntity(entity);
    models.unregisterEntity(entity);
    //camera
}

void RenderEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    lights.addComponent(entity, component_type);
    models.addComponent(entity, component_type);
    //camera
}

void renderTexture(Program& p, const Texture& t, const Camera& c, const Mesh& m, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //material
    // bind the texture and set the "tex" uniform in the fragment shader
    TextureContext tex{t, GL_TEXTURE0};
    p.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    
    //always
    m.draw(p);
}

void renderLight(Program& p, const Camera& c, const Mesh& m, const Light& l, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(modelView))); //not part of light, but only needed for lights so far
    
    //material    
    model.attach(p);
    
    //light
    l.attach(p, c);
    
    p.setUniform("normal", normal); //not part of light, but only needed for lights so far
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //always
    m.draw(p);
}

void render(Program& pt, Program& pl, const Camera& c,
            const NodeCache<Light>& lights, Model& model)
{
    //static settings, really
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    bool first = true;
    for(auto& light : lights)
    {
        if(first)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            first = false;
        }
        else
        {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        }
        renderLight(pl, c, *model.model.mesh, *light, model);
    }
    
    glBlendFunc(GL_ZERO,GL_SRC_COLOR);
    renderTexture(pt, *model.model.texture, c, *model.model.mesh, model);
    
    glDisable(GL_BLEND);
}

void RenderEngine::execute()
{
    glClearColor(0.0, 0.0, 0.0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(auto& model : models)
    {
        render(_pt, _pl, *_camera, lights, *model);
    }
    
    glfwSwapBuffers();
}