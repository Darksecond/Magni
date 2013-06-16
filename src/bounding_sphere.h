#pragma once

#include "bounding_volume.h"
#include "spatial.h"

#include <GLM/glm.hpp>

class bounding_sphere : public bounding_volume
{
public:
    bounding_sphere(float radius, const spatial& local_center = spatial());
    virtual bool intersects(const bounding_volume& volume) const;
    virtual bool intersects_sphere(const bounding_sphere& volume) const;
    virtual void update_global(const spatial& new_global);
private:
    spatial _local_center;
    spatial _global_center;
    float _radius;
};