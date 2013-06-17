#pragma once

#include "renderer.h"
#include "collision_system.h"

#include <string>
#include <memory>
#include <list>

namespace Ymir
{
    class DirectoryManifest;
};
class game_object;
class scene;
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
    
    inline std::shared_ptr<scene> active_scene();
private:
    game();
    bool _running;
    
    std::shared_ptr<Ymir::DirectoryManifest> _manifest;
    
    collision_system _collider;
    renderer _renderer;
    
    std::shared_ptr<scene> _active_scene;
};

//TEMPLATE & INLINE METHODS

inline std::shared_ptr<scene> game::active_scene()
{
    return _active_scene;
}
