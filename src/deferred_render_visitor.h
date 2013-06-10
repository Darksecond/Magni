#pragma once

#include "game_object_visitor.h"
#include "render_frame.h"
#include "gbuffer.h"

#include "Program.h"

#include <memory>

class deferred_render_visitor : public game_object_visitor
{
public:
    deferred_render_visitor(std::shared_ptr<Ymir::Program> _texture_program, glm::ivec2 SCREEN_SIZE);
    virtual ~deferred_render_visitor();
    
    virtual void start_visit(light&);
    virtual void start_visit(camera&);
    virtual void start_visit(model&);
    
    inline void set_frame(render_frame* frame);
    
    void start_frame();
    void end_frame();
private:
    render_frame* _frame;
    std::shared_ptr<Ymir::Program> _texture_program;
    gbuffer _gbuffer;
    glm::ivec2 _SCREEN_SIZE;
};

//INLINE & TEMPLATE METHODS

void deferred_render_visitor::set_frame(render_frame* frame)
{
    _frame = frame;
}