#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

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

glm::mat4 Model::matrix()
{
    glm::mat4 matrix = glm::mat4_cast(spatial->direction);
    matrix = glm::translate(matrix, -spatial->position);
    return matrix;
}