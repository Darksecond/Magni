#pragma once

#include "spatial.h"
#include "game_object_visitor.h"
#include "behaviour.h"
#include "bounding_volume.h"

#include <string>
#include <vector>
#include <memory>

class game_object
{
public:
    explicit game_object(const std::string& name, const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
    virtual ~game_object() = default;
    
    void add(std::shared_ptr<game_object> child);
    
    virtual void accept(game_object_visitor& visitor);
    
    const std::string& name() const;
    
    spatial& local();
    spatial& global();
    std::shared_ptr<bounding_volume> collider();
    behaviour* get_behaviour();
    
    void set_behaviour(std::unique_ptr<behaviour> b);
    void set_collider(std::shared_ptr<bounding_volume> bv);
    void update_global(const spatial& new_global);
    void update();
protected:
    std::vector<std::shared_ptr<game_object>> _children;
private:
    const std::string _name;
    spatial _local_spatial;
    spatial _global_spatial;
    std::unique_ptr<behaviour> _behaviour;
    std::shared_ptr<bounding_volume> _bounding_volume;
};