#pragma once

#include "../Entity.h"
#include "../ModelComponent.h"
#include "../SpatialComponent.h"
#include "../Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class Model
    {
    public:
        ModelComponent& model;
        SpatialComponent& spatial;

        static std::unique_ptr<Model> fromEntity(const Entity& e);
        static bool needsComponent(const BaseComponent::Type type);

        Model(ModelComponent& m, SpatialComponent& s) : model(m), spatial(s)
        {
        }

        void attach(Program& p) const;

        bool operator==(const Model& other) const;
    };
};
