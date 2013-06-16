#pragma once

class bounding_sphere;
class spatial;
class bounding_volume
{
public:
    virtual bool intersects(const bounding_volume& volume) const = 0;
    virtual bool intersects_sphere(const bounding_sphere& volume) const = 0;
    virtual void update_global(const spatial& new_global) = 0;
};