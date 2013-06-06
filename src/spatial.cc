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

glm::mat4 spatial::matrix() const
{
    return _matrix;
}

glm::vec3 spatial::translation() const
{
    return glm::vec3(_matrix[3]);
}

spatial& spatial::operator*=(const spatial& b)
{
    _matrix * b.matrix();
    return *this;
}

spatial operator*(const spatial& a, const spatial& b)
{
    return spatial(a.matrix() * b.matrix());
}