#pragma once

#include "game_object_visitor.h"
#include "spatial.h"

#include <stack>
#include <deque>

class spatial_updater_visitor : public game_object_visitor
{
public:
    spatial_updater_visitor();
    
    virtual void start_visit(game_object&);
    virtual void start_visit(light&);
    virtual void start_visit(camera&);
    virtual void start_visit(model&);
    
    virtual void end_visit(game_object&);
    virtual void end_visit(light&);
    virtual void end_visit(camera&);
    virtual void end_visit(model&);
private:
    void start(game_object&);
    void end(game_object&);
    std::stack<spatial> _stack;
};