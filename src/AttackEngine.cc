#include "AttackEngine.h"
#include "AttackComponent.h"
#include "HealthComponent.h"
#include "ModelComponent.h"
#include "SpatialComponent.h"

#include <iostream>
#include <stdexcept>

using namespace Ymir;

AttackEngine::AttackEngine(ResourceManager<Mesh>& mn, ResourceManager<Texture>& tm) : mesh_manager(mn), texture_manager(tm)
{
    _attack_animation_entity = nullptr;
    _attack_animation_life = 0;
}

void AttackEngine::registerEntity(Entity& entity)
{
}

void AttackEngine::unregisterEntity(Entity& entity)
{
}

void AttackEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
}

void AttackEngine::update(int pass, double delta)
{
    if(_attack_animation_entity != nullptr)
    {
        _attack_animation_life -= delta;
        _attack_animation_entity->component<SpatialComponent>()->scale *= 0.99;
        if(_attack_animation_life <= 0)
        {
            //delete animation
            scene->deleteEntity(_attack_animation_entity);
            _attack_animation_entity = nullptr;
        }
    }
}

void AttackEngine::attack(Entity& attackee, const Entity& attacker)
{
    if(scene == nullptr)
        throw std::runtime_error("No scene specified, cannot attack");

    HealthComponent* health = attackee.component<HealthComponent>();
    AttackComponent* attack = attacker.component<AttackComponent>();
    SpatialComponent* spatialAttackee = attackee.component<SpatialComponent>();
    SpatialComponent* spatialAttacker = attacker.component<SpatialComponent>();

    health->health -= attack->attack;

    std::cout << "Shoot lazors from: "
            << spatialAttacker->get_position().x << " "
            << spatialAttacker->get_position().y << " "
            << spatialAttacker->get_position().z << " "
            << "to position " << " "
            << spatialAttackee->get_position().x << " "
            << spatialAttackee->get_position().y << " "
            << spatialAttackee->get_position().z
            << std::endl;

    if(health->health <= 0)
    {
        if(_attack_animation_entity == nullptr) {
            //show attack 'animation'
            _attack_animation_entity = &scene->assign("attack_animation");
            //add stuff!
            _attack_animation_entity->assign<ModelComponent>(mesh_manager.resource("explosion.obj"), texture_manager.resource("explosion.png"));
            auto& spatial = _attack_animation_entity->assign<SpatialComponent>(glm::vec3(0));
            spatial.set_position(attackee.component<SpatialComponent>()->get_position());
            spatial.scale = glm::vec3(1);
            //spatial.position = attackee.component<SpatialComponent>()->position;
            _attack_animation_life = attack->duration;
        }

        //unit is dead, delete?
        scene->deleteEntity(&attackee);
    }
}
