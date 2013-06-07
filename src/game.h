#pragma once

#include "game_object.h"

#include <string>
#include <memory>

class game
{
public:
    game();
    
    void boot(const std::string& resource_dir);
    void build();
    void run();
    void stop();
private:
    bool _running;
    std::shared_ptr<game_object> _world;
};