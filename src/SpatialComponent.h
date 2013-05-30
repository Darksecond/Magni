#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class SpatialComponent : public Component<SpatialComponent>
    {
        glm::quat direction;
    public:
        glm::vec3 position;
        glm::vec3 scale;

        SpatialComponent(SpatialComponent&) = delete;

        explicit SpatialComponent(glm::vec3 position);

        const glm::vec3 directionEuler() const;

        void setDirection(const glm::quat& quat);
        void setDirection(const glm::vec3& euler);

        glm::mat4 matrix() const;

        glm::mat4 orientation() const;
        glm::vec3 globalPosition() const;
        glm::quat globalDirection() const;
        glm::vec3 forward() const;
        glm::vec3 forward(float amount) const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        inline void set_position(const glm::vec3& pos) { position = pos; }
        inline glm::vec3 get_position() { return position; }

        inline glm::quat get_direction() { return direction; }
    };
};
