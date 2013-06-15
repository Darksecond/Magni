#pragma once

#include "Program.h"
#include "Mesh.h"
#include "render_frame.h"
#include "deferred_render_visitor.h"

#include <GLM/glm.hpp>
#include <memory>

class game_object;
class deferred_render_visitor;
class renderer
{
public:
    renderer(const glm::ivec2& screen_size);
    void boot();
    void shutdown();
    
    bool step(std::shared_ptr<game_object> world);
private:
    const glm::ivec2 SCREEN_SIZE;
    std::unique_ptr<deferred_render_visitor> _deferred_renderer;
};