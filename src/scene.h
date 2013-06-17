#pragma once

#include <memory>
#include <list>

class game_object;
class scene
{
public:
    scene();
    scene(std::shared_ptr<game_object>& world);
    
    std::shared_ptr<game_object> add_game_object(std::shared_ptr<game_object> object);
    std::shared_ptr<game_object> add_game_object(std::shared_ptr<game_object> object, std::shared_ptr<game_object> parent);
    
    std::shared_ptr<game_object> get_by_name(const std::string& name);
    //TODO std::list<std::shared_ptr<game_object>> get_by_position(const glm::vec3& position, float range);
    
    inline std::list<std::shared_ptr<game_object>>& linear_view();
    inline std::shared_ptr<game_object> scene_graph_view();
private:
    std::shared_ptr<game_object> _world;
    std::list<std::shared_ptr<game_object>> _linear_view;
};

//TEMPLATE & INLINE METHODS

std::list<std::shared_ptr<game_object>>& scene::linear_view()
{
    return _linear_view;
}

inline std::shared_ptr<game_object> scene::scene_graph_view()
{
    return _world;
}
