#pragma once

#include "game_object.h"

//TODO redo, cc file
class light : public game_object
{
public:
    explicit inline light(const std::string& name) : game_object(name) {}
    
    inline virtual void accept(game_object_visitor& v)
    {
        v.visit(*this);
        for(auto child : _children)
            child->accept(v);
    }
};