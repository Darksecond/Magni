#include "Model.h"

using namespace Ymir;

std::unique_ptr<Model> Model::fromEntity(const Entity& entity)
{
    //if entity has both Light and Spatial, make a Light
    ModelComponent* modelC = entity.component<ModelComponent>();
    SpatialComponent* spatialC = entity.component<SpatialComponent>();
    if(modelC != nullptr && spatialC != nullptr)
    {
        return std::unique_ptr<Model>{new Model{*modelC, *spatialC}};
    }
    return nullptr;
}

bool Model::needsComponent(const BaseComponent::Type type)
{
    if(type == ModelComponent::type() || type == SpatialComponent::type())
        return true;
    return false;
}

void Model::attach(Program &p) const
{
    p.setUniform("material.diffuse", glm::vec3(0.3, 0.3, 0.3));
    p.setUniform("material.ambient", glm::vec3(0.1, 0.1, 0.1));
    p.setUniform("material.specular", glm::vec3(1.0, 1.0, 1.0));
    p.setUniform("material.emissive", glm::vec3(0.05, 0.05, 0.05));
    p.setUniform("material.shininess", 90.0f);
}

bool Model::operator==(const Model& other) const
{
    return &model == &other.model && &spatial == &other.spatial; //not tested yet
}