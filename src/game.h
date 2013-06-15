#pragma once

#include "renderer.h"

#include <string>
#include <memory>
#include <list>

namespace Ymir
{
    class DirectoryManifest;
};
class game_object;
class game
{
public:
    static game& instance();
    
    game(const game&) = delete;
    game(game&&) = delete;
    game& operator=(const game&) = delete;
    game& operator=(game&&) = delete;
    
    void boot(const std::string& resource_dir);
    void build();
    void run();
    void shutdown();
    void stop();
    
    std::shared_ptr<game_object> add_game_object(std::shared_ptr<game_object> object);
    std::shared_ptr<game_object> add_game_object(std::shared_ptr<game_object> object, std::shared_ptr<game_object> parent);
    
    std::shared_ptr<game_object> get_by_name(const std::string& name);
    //TODO: std::list<std::shared_ptr<game_object>> get_by_position(const glm::vec3& position, float range);
private:
    game();
    bool _running;
    std::shared_ptr<game_object> _world;
    std::list<std::shared_ptr<game_object>> _linear_view;
    
    std::shared_ptr<Ymir::DirectoryManifest> _manifest;
    
    renderer _renderer;
};