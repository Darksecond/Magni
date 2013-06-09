#include "deferred_render_visitor.h"

#include "commands.h"
#include "camera.h"
#include "model.h"
#include "light.h"

deferred_render_visitor::deferred_render_visitor(std::shared_ptr<Ymir::Program> texture_program) :
    _texture_program(texture_program)
{
    //g-buffer
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
    _frame->add<0, render_commands::clear_color>(0, 0, 0, 1);
    _frame->add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _frame->add<0, render_commands::bind_program>(_texture_program);
}

void deferred_render_visitor::end_frame()
{
}