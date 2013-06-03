#include "MoveEngine.h"
#include "AttackComponent.h"
#include "HealthComponent.h"
#include "ModelComponent.h"
#include "SpatialComponent.h"
#include "MoveComponent.h"
#include <iostream>
#include <stdexcept>

using namespace Ymir;

MoveEngine::MoveEngine()
{
}

void MoveEngine::registerEntity(Entity& entity)
{
    entities.push_back(&entity);
}
void MoveEngine::unregisterEntity(Entity& entity)
{
    std::vector <Entity*>::iterator it;
    for(it = this->entities.begin(); it != this->entities.end(); ++it){
          if((*it)->id == entity.id) {
            break;
        }
    }
    entities.erase(it);
}

void MoveEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
}

void MoveEngine::update(int pass, double delta)
{
    if (pass = -1) {
        for(std::vector<Entity*>::const_iterator iter = this->entities.begin(), end= this->entities.end(); iter != end; ++iter){
            if((*iter) != nullptr) {
                auto spatial = (*iter)->component<SpatialComponent>();
                if(spatial != nullptr) {
                    auto moveC = (*iter)->component<MoveComponent>();
                    if(moveC != nullptr) {
                        if(moveC->tiles != nullptr) {
                            float speed = moveC->speed;
                            int rspeed= moveC->rotationSpeed;
                            std::vector<Tile> *route = moveC->tiles;
                            if(route->size() >= 1){
                                glm::vec3 nextPoint = route->at(0).centerpoint;
                                glm::mat4 direction = glm::inverse(glm::lookAt(nextPoint, spatial->position, glm::vec3(0.0f,1.0f,0.0f)));
                                glm::quat q = glm::toQuat(direction);
                                spatial->setDirection(q);
                                spatial->set_position(spatial->get_position() + spatial->forward(speed));
                                if(glm::distance(spatial->position,nextPoint) < 0.05f){
                                    spatial->set_position(nextPoint);
                                    route->erase(route->begin(),route->begin()+1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
