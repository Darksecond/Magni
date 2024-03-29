#pragma once

#include <GLM/glm.hpp>

class spatial
{
public:
    explicit spatial(const glm::vec3& translation = glm::vec3(0.0f));
    explicit spatial(const glm::mat4& matrix);
    
    void translate(const glm::vec3& translation);
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& new_scale);
    void look_at(const glm::vec3& at, const glm::vec3& up);
    
    glm::mat4 matrix() const;
    
    glm::vec3 translation() const;
    glm::vec4& translation_vec4();
    
    void identity();
    
    spatial& operator*=(const spatial& b);
private:
    glm::mat4 _matrix;
};

spatial operator*(const spatial& a, const spatial& b);