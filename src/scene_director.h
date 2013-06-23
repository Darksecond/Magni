#pragma once

#include <memory>
#include <string>

class scene;
class scene_builder;
class scene_director
{
    std::shared_ptr<scene> _scene;
    std::shared_ptr<scene_builder> _builder;
public:
    scene_director(std::shared_ptr<scene_builder> builder);
    void construct(const std::string& identifier);
    inline std::shared_ptr<scene> get_scene() { return _scene; }
};