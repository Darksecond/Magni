#include "Model.h"

std::unique_ptr<Model> Model::fromEntity(const Entity& entity)
{
    //if entity has both Light and Spatial, make a Light
    std::shared_ptr<ModelComponent> modelC = entity.component<ModelComponent>();
    std::shared_ptr<SpatialComponent> spatialC = entity.component<SpatialComponent>();
    if(modelC != nullptr && spatialC != nullptr)
    {
        return std::unique_ptr<Model>{new Model{modelC, spatialC}};
    }
    return std::unique_ptr<Model>{};
}

void Model::attach(Program &p)
{
    p.setUniform("material.diffuse", glm::vec3(0.3, 0.3, 0.3));
    p.setUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
    p.setUniform("material.specular", glm::vec3(1.0, 1.0, 1.0));
    p.setUniform("material.emissive", glm::vec3(0.1, 0.1, 0.1));
    p.setUniform("material.shininess", 90.0f);
}