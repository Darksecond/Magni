#include "spatial.h"

#include <GLm/ext.hpp>

spatial::spatial(const glm::vec3& translation) : _matrix(glm::translate(translation))
{
}

spatial::spatial(const glm::mat4& matrix) : _matrix(matrix)
{
}

void spatial::translate(const glm::vec3& translation)
{
    _matrix = glm::translate(_matrix, translation);
}

void spatial::rotate(float angle, const glm::vec3& axis)
{
    _matrix = glm::rotate(_matrix, angle, axis);
}

void spatial::scale(const glm::vec3& new_scale)
{
    _matrix = glm::scale(_matrix, new_scale);
}

void spatial::look_at(const glm::vec3& at, const glm::vec3& up)
{
    _matrix = glm::inverse(glm::lookAt(translation(), at, up));
}

glm::mat4 spatial::matrix() const
{
    return _matrix;
}

glm::vec3 spatial::translation() const
{
    return glm::vec3(_matrix[3]);
}

glm::vec4& spatial::translation_vec4()
{
    return _matrix[3];
}

void spatial::identity()
{
    _matrix = glm::mat4();
}

spatial& spatial::operator*=(const spatial& b)
{
    _matrix = _matrix * b.matrix();
    return *this;
}

spatial operator*(const spatial& a, const spatial& b)
{
    return spatial(a.matrix() * b.matrix());
}