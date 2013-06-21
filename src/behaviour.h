#pragma once
#include <iostream>

class game_object;
class behaviour
{
public:
    behaviour(game_object& go) : _parent(go) {}
    virtual ~behaviour() = default;
    
    virtual void update() = 0;
    virtual void on_collision(game_object& other) { std::cout << "collision" << std::endl;};
    
protected:
    game_object& _parent;
};