#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class SpatialComponent : public Component<SpatialComponent>
    {
    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat direction;
                
        SpatialComponent(SpatialComponent&) = delete;
        
        explicit SpatialComponent(glm::vec3 position);
        
        const glm::vec3 directionEuler() const;
        
        void setDirection(glm::quat& quat);
        void setDirection(glm::vec3& euler);
        
        glm::mat4 matrix() const;
        
        glm::mat4 orientation() const;
        glm::vec3 globalPosition() const;
        glm::quat globalDirection() const;
        glm::vec3 forward() const;
        glm::vec3 right() const;
        glm::vec3 up() const;
    };
};