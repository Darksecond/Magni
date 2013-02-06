#include "RenderEngine.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "Light.h"
#include "LightComponent.h"
#include "SpatialComponent.h"

RenderEngine::RenderEngine(Program& pt, Program& pl) : _pt{pt}, _pl{pl}, lights{}, models{}
{
}

void RenderEngine::registerEntity(Entity& entity)
{
    std::unique_ptr<Light> lightNode = entity.node<Light>();
    if(lightNode)
    {
        lights.push_back(std::move(lightNode));
    }
    
    std::unique_ptr<Camera> cameraNode = entity.node<Camera>();
    if(cameraNode)
    {
        _camera = std::move(cameraNode);
    }
    
    std::unique_ptr<Model> modelNode = entity.node<Model>();
    if(modelNode)
    {
        models.push_back(std::move(modelNode));
    }
    
    //if entity has both Model and Spatial, make a Model
    //if entity has both Camera and Spatial, make a Camera
}

void RenderEngine::unregisterEntity(Entity& entity)
{
    std::unique_ptr<Light> lightNode = entity.node<Light>();
    if(lightNode)
    {
        lights.erase(std::remove_if(lights.begin(), lights.end(),
                                    [&](const std::unique_ptr<Light>& i)
                                    {return *lightNode == *i; })
                     , lights.end());
    }
}

void renderTexture(Program& p, const Texture& t, const Camera& c, const Mesh& m, glm::mat4 model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    //glm::mat4 model = glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(1,0,0));
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model;
    
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

void renderLight(Program& p, const Camera& c, const Mesh& m, const Light& l, glm::mat4 model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    //glm::mat4 model = glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(1,0,0));
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model;
    
    glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(modelView))); //not part of light, but only needed for lights so far
    
    //material
    p.setUniform("material.diffuse", glm::vec3(0.3, 0.3, 0.3));
    p.setUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
    p.setUniform("material.specular", glm::vec3(1.0, 1.0, 1.0));
    p.setUniform("material.emissive", glm::vec3(0.0, 0.0, 0.0));
    p.setUniform("material.shininess", 90.0f);
    
    //light
    l.attach(p, c);
    
    p.setUniform("normal", normal); //not part of light, but only needed for lights so far
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //always
    m.draw(p);
}

void render(Program& pt, Program& pl, const Texture& t, const Camera& c, const Mesh& m, glm::mat4 model,
            std::vector<std::unique_ptr<Light>>& lights)
{
    //static settings, really
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    for(auto& light : lights)
    {
        glBlendFunc(GL_ONE,GL_ONE);
        renderLight(pl, c, m, *light, model);
    }
    
    glBlendFunc(GL_ZERO,GL_SRC_COLOR);
    renderTexture(pt, t, c, m, model);
    
    glDisable(GL_BLEND);
}

void RenderEngine::execute()
{
    glClearColor(0.0, 0.0, 0.0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(auto& model : models)
    {
        render(_pt, _pl, *model->model->texture, *_camera, *model->model->mesh, model->matrix(), lights);
    }
    
    glfwSwapBuffers();
}