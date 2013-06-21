#include "aabb.h"
#include "bounding_sphere.h"

#include <iostream>

bool aabb::intersects(const bounding_volume& volume) const
{
    return volume.intersects_aabb(*this);
}

bool aabb::intersects_sphere(const bounding_sphere& volume) const
{
    //aabb global = recalculate();
    //TODO
    return false;
}

bool aabb::intersects_aabb(const aabb& volume) const
{
    aabb a = recalculate();
    aabb b = volume.recalculate();
    
    return !(a._min.x > b._max.x || a._max.x < b._min.x ||
           a._min.y > b._max.y || a._max.y < b._min.y ||
           a._min.z > b._max.z || a._max.z < b._min.z);
}

void aabb::update_global(const spatial& new_global)
{
    _global = new_global;
}

aabb aabb::recalculate() const
{
    //calculate world space aabb
    aabb retval;
    
    const glm::vec3& middle = center();
    const glm::vec3& diag = middle - _max;
    const glm::mat4& mat = _global.matrix();
    
    glm::vec3 edges[8];
    
    edges[0] = glm::vec3(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
    edges[1] = glm::vec3(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
    edges[2] = glm::vec3(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
    edges[3] = glm::vec3(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
    edges[4] = glm::vec3(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
    edges[5] = glm::vec3(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
    edges[6] = glm::vec3(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
    edges[7] = glm::vec3(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
    
    edges[0] = glm::vec3(mat * glm::vec4(edges[0], 1.0f));
    edges[1] = glm::vec3(mat * glm::vec4(edges[1], 1.0f));
    edges[2] = glm::vec3(mat * glm::vec4(edges[2], 1.0f));
    edges[3] = glm::vec3(mat * glm::vec4(edges[3], 1.0f));
    edges[4] = glm::vec3(mat * glm::vec4(edges[4], 1.0f));
    edges[5] = glm::vec3(mat * glm::vec4(edges[5], 1.0f));
    edges[6] = glm::vec3(mat * glm::vec4(edges[6], 1.0f));
    edges[7] = glm::vec3(mat * glm::vec4(edges[7], 1.0f));
    
    retval.reset(edges[0]);
    retval.add_point(edges[1]);
    retval.add_point(edges[2]);
    retval.add_point(edges[3]);
    retval.add_point(edges[4]);
    retval.add_point(edges[5]);
    retval.add_point(edges[6]);
    retval.add_point(edges[7]);
    
    return retval;
}