#pragma once

#include <memory>

class scene;
class collision_system
{
public:
    bool step(std::shared_ptr<scene>& active_scene);
};