#pragma once

class game_object;
class light;
class game_object_visitor
{
public:
    virtual ~game_object_visitor() = default;
    
    virtual void start_visit(game_object&) {};
    virtual void end_visit(game_object&) {};
    
    virtual void start_visit(light&) {};
    virtual void end_visit(light&) {};
};