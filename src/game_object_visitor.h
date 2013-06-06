#pragma once

class game_object;
class light;
class game_object_visitor
{
public:
    virtual ~game_object_visitor() = default;
    virtual void visit(game_object&) = 0;
    virtual void visit(light&) = 0;
};