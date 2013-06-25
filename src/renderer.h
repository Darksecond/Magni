#pragma once

#include "Program.h"
#include "Mesh.h"
#include "render_frame.h"
#include "deferred_render_visitor.h"
#include "text.h"
#include "module.h"

#include <GLM/glm.hpp>
#include <memory>

class game_object;
class deferred_render_visitor;
class scene;
class renderer : public module
{
public:
    renderer(const glm::ivec2& screen_size);
    virtual void boot();
    virtual void shutdown();
    virtual void build();
    virtual bool step(std::shared_ptr<scene>& active_scene);
    virtual void on_event(const event_t type, void* data);
private:
    const glm::ivec2 SCREEN_SIZE;
    std::unique_ptr<deferred_render_visitor> _deferred_renderer;
    std::list<std::shared_ptr<text>> _texts;
    
    std::shared_ptr<Ymir::Program> _overlay_program;
    std::shared_ptr<Ymir::Texture> _holstein;
};