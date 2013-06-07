#pragma once

class game_object;
class light;
class camera;
class model;

class game_object_visitor
{
public:
    virtual ~game_object_visitor() = default;
    
    virtual void start_visit(game_object&) {};
    virtual void end_visit(game_object&) {};
    
    virtual void start_visit(light&) {};
    virtual void end_visit(light&) {};
    
    virtual void start_visit(camera&) {};
    virtual void end_visit(camera&) {};
    
    virtual void start_visit(model&) {};
    virtual void end_visit(model&) {};
};