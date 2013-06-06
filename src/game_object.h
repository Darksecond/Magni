#pragma once

#include "spatial.h"
#include "game_object_visitor.h"

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
protected:
    std::vector<std::shared_ptr<game_object>> _children;
private:
    const std::string _name;
    spatial _spatial;
};