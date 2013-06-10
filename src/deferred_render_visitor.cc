#include "deferred_render_visitor.h"

#include "commands.h"
#include "gbuffer_commands.h"
#include "camera.h"
#include "model.h"
#include "light.h"

deferred_render_visitor::deferred_render_visitor(std::shared_ptr<Ymir::Program> texture_program, glm::ivec2 SCREEN_SIZE) :
    _texture_program(texture_program), _gbuffer(SCREEN_SIZE), _SCREEN_SIZE(SCREEN_SIZE)
{
}

deferred_render_visitor::~deferred_render_visitor()
{
}

void deferred_render_visitor::start_visit(light& l)
{
}

void deferred_render_visitor::start_visit(camera& c)
{
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("projection", c.projection());
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("view", glm::inverse(c.global().matrix()));
}

void deferred_render_visitor::start_visit(model& m)
{
    _frame->add<1, render_commands::draw_model>(m.mesh(), m.material(), m.global().matrix());
}

void deferred_render_visitor::start_frame()
{
    _frame->add<0, render_commands::bind_program>(_texture_program);
    _frame->add<0, render_commands::bind_gbuffer>(&_gbuffer, GL_DRAW_FRAMEBUFFER);
    
    gbuffer::GBUFFER_TEXTURE_TYPE bufs[] = {gbuffer::GBUFFER_TEXTURE_TYPE_POSITION, gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE, gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL};
    _frame->add<0, render_commands::set_draw_buffers>(3, bufs);
    
    _frame->add<0, render_commands::clear_color>(0, 0, 0, 1);
    _frame->add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void deferred_render_visitor::end_frame()
{
    _frame->add<2, render_commands::unbind_gbuffer>(GL_DRAW_FRAMEBUFFER);
    _frame->add<2, render_commands::bind_gbuffer>(&_gbuffer, GL_READ_FRAMEBUFFER);
    _frame->add<2, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    _frame->add<2, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_left);
    _frame->add<2, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    _frame->add<2, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_right);
    _frame->add<2, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    _frame->add<2, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::bottom_left);
}