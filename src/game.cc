#include "game.h"

#include "spatial_updater_visitor.h"

game::game() : _running(true), _world()
{
}

void game::boot(const std::string& resource_dir)
{
}

void game::build()
{
    _world = std::make_shared<game_object>("world");
}

void game::run()
{
    while(_running)
    {
        //TODO moar!
        
        spatial_updater_visitor spatial_updater;
        _world->accept(spatial_updater);
        
        stop(); //TODO TEMP REMOVE
    }
}

void game::stop()
{
    _running = false;
}