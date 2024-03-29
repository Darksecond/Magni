#pragma once

#include "spatial.h"
#include "game_object_visitor.h"
#include "behaviour.h"
#include "bounding_volume.h"
#include "events.h"
#include "module.h"

#include <string>
#include <vector>
#include <list>
#include <memory>

class game_object
{
public:
    explicit game_object(const std::string& name, const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
    virtual ~game_object() = default;
    
    void add(std::shared_ptr<game_object> child);
    void remove(std::shared_ptr<game_object> object);
    
    virtual void accept(game_object_visitor& visitor);
    
    void notify(event_t type, void* data);
    void on_event(event_t type, void* data);
    void add_listener(module* m);
    
    const std::string& name() const;
    
    spatial& local();
    spatial& global();
    std::shared_ptr<bounding_volume> collider();
    behaviour* get_behaviour();
    
    void set_behaviour(std::shared_ptr<behaviour> b);
    void set_collider(std::shared_ptr<bounding_volume> bv);
    void update_global(const spatial& new_global);
    void update();
protected:
    std::list<std::shared_ptr<game_object>> _children;
private:
    const std::string _name;
    spatial _local_spatial;
    spatial _global_spatial;
    std::shared_ptr<behaviour> _behaviour;
    std::shared_ptr<bounding_volume> _bounding_volume;
    std::list<module*> _listeners;
};