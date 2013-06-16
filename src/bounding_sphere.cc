#include "bounding_sphere.h"

bounding_sphere::bounding_sphere(float radius, const spatial& local_center) : _local_center(local_center), _global_center(), _radius(radius)
{
}

bool bounding_sphere::intersects(const bounding_volume& volume) const
{
    return volume.intersects_sphere(*this);
}

bool bounding_sphere::intersects_sphere(const bounding_sphere& other) const
{
    float distance = glm::distance(_global_center.translation(), other._global_center.translation());
    if(distance < _radius + other._radius)
    {
        return true;
    }
    return false;
}

void bounding_sphere::update_global(const spatial& new_global)
{
    _global_center = new_global * _local_center;
}
