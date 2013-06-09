#pragma once

class game_object;
class behaviour
{
public:
    behaviour(game_object& go) : _parent(go) {}
    virtual ~behaviour() = default;
    
    virtual void update() = 0;
    
protected:
    game_object& _parent;
};