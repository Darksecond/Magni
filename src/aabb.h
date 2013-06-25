#pragma once

#include "bounding_volume.h"
#include "spatial.h"

#include <GlM/glm.hpp>
#include <iostream>

class aabb : public bounding_volume
{
    aabb recalculate() const;
public:
    glm::vec3 _min;
    glm::vec3 _max;
    spatial _global;
    
    aabb() : _min(-1), _max(1), _global() {}
    explicit aabb(const glm::vec3& point) : _min(point), _max(point), _global() {}
    aabb(const aabb& other) : _min(other._min), _max(other._max), _global(other._global) {}
    aabb(aabb&& other) : _min(other._min), _max(other._max), _global(other._global) {}
    aabb(const glm::vec3& min, const glm::vec3& max) : _min(min), _max(max) {}
    
    inline bool operator==(const aabb& other) const
    {
        return (_min == other._min && _max == other._max);
    }
    inline bool operator!=(const aabb& other) const
    {
        return !(_min == other._min && _max == other._max);
    }
    
    virtual bool intersects(const bounding_volume& volume) const;
    virtual bool intersects_sphere(const bounding_sphere& volume) const;
    virtual bool intersects_aabb(const aabb& volume) const;
    virtual void update_global(const spatial& new_global);
    
    void reset(const glm::vec3& point)
    {
        _min = _max = point;
    }
    
    void add_point(const glm::vec3& point)
    {
        if(point.x > _max.x) _max.x = point.x;
        if(point.y > _max.y) _max.y = point.y;
        if(point.z > _max.z) _max.z = point.z;
        
        if(point.x < _min.x) _min.x = point.x;
        if(point.y < _min.y) _min.y = point.y;
        if(point.z < _min.z) _min.z = point.z;
    }
    
    glm::vec3 center() const
    {
        return (_min + _max) / 2.0f;
    }
    
    glm::vec3 extent() const
    {
        return _max - _min;
    }
};