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
}

void MoveEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
}

void MoveEngine::update(int pass, double delta)
{
    if (pass = -1) {
        for(std::vector<Entity*>::const_iterator iter = this->entities.begin(), end= this->entities.end(); iter != end; ++iter){
            auto spatial = (*iter)->component<SpatialComponent>();
            if(spatial != nullptr) {
                auto moveC = (*iter)->component<MoveComponent>();
                if(moveC != nullptr) {
                    int speed = moveC->speed;
                    int rspeed= moveC->rotationSpeed;
                    std::vector<Tile> *route = moveC->tiles;
                    if(route->size() >= 1){
                        spatial->forward(delta*speed);
                        spatial->position = route->at(0).centerpoint;
                        route->erase(route->begin(),route->begin()+1);
                        moveC->tiles = route;
                    }
                }
            }
        }
    }
}
