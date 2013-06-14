#include "deferred_render_visitor.h"

#include "commands.h"
#include "gbuffer_commands.h"
#include "deferred_commands.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "resource_factory.h"

deferred_render_visitor::deferred_render_visitor(glm::ivec2 SCREEN_SIZE) :
_gbuffer(SCREEN_SIZE),
_SCREEN_SIZE(SCREEN_SIZE),
_geometry_program(resource_factory::instance().resource<Ymir::Program>("geometry", "program")),
_point_light_program(resource_factory::instance().resource<Ymir::Program>("point_lighting", "program")),
_sphere_mesh(resource_factory::instance().resource<Ymir::Mesh>("sphere.obj", "mesh"))
{
    //0 == prepare geometry pass
    //1 == draw geometry pass
    //2 == prepare (point) lighting pass
    //3 == draw (point) lighting pass
    //4 == blit final
}

deferred_render_visitor::~deferred_render_visitor()
{
}


void deferred_render_visitor::start_visit(light& l)
{
    spatial s = l.global();
    float scale = l.radius();
    s.scale(glm::vec3(scale, scale, scale));
    _frame->add<3, render_commands::draw_light>(_sphere_mesh, s, l.radius());
    //_frame->add<1, render_commands::draw_model>(_sphere_mesh, nullptr, s.matrix());
}

void deferred_render_visitor::start_visit(camera& c)
{
    //geometry pass
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("projection", c.projection());
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("view", glm::inverse(c.global().matrix()));
    //lighting pass
    _frame->add<2, render_commands::set_uniform<glm::mat4>>("view", glm::inverse(c.global().matrix()));
    _frame->add<2, render_commands::set_uniform<glm::mat4>>("projection", c.projection());
}

void deferred_render_visitor::start_visit(model& m)
{
    _frame->add<1, render_commands::draw_model>(m.mesh(), m.material(), m.global().matrix());
}

void deferred_render_visitor::start_frame()
{
    //prepare for geometry pass
    _frame->add<0, render_commands::bind_program>(_geometry_program);
    _frame->add<0, render_commands::unbind_gbuffer>(GL_READ_FRAMEBUFFER);
    _frame->add<0, render_commands::bind_gbuffer>(&_gbuffer, GL_DRAW_FRAMEBUFFER);
    
    gbuffer::GBUFFER_TEXTURE_TYPE bufs[] = {
        gbuffer::GBUFFER_TEXTURE_TYPE_POSITION,
        gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE,
        gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL
    };
    _frame->add<0, render_commands::set_draw_buffers>(3, bufs);
    _frame->add<0, render_commands::clear_color>(0, 0, 0, 1);
    _frame->add<0, render_commands::set_depth>(true, true);
    _frame->add<0, render_commands::set_blend>(false);
    _frame->add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //prepare for (point) lighting pass
     _frame->add<2, render_commands::bind_program>(_point_light_program);
     _frame->add<2, render_commands::set_draw_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<2, render_commands::clear>(GL_COLOR_BUFFER_BIT);
    _frame->add<2, render_commands::set_depth>(false, false);
    _frame->add<2, render_commands::set_blend>(true, GL_FUNC_ADD, GL_ONE, GL_ONE);
    _frame->add<2, render_commands::set_uniform<glm::vec2>>("screen_size", (glm::vec2)_SCREEN_SIZE);
    _frame->add<2, render_commands::bind_gbuffer_texture>(&_gbuffer, "g_diffuse", gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE, GL_TEXTURE0);
    _frame->add<2, render_commands::bind_gbuffer_texture>(&_gbuffer, "g_position", gbuffer::GBUFFER_TEXTURE_TYPE_POSITION, GL_TEXTURE1);
    _frame->add<2, render_commands::bind_gbuffer_texture>(&_gbuffer, "g_normal", gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL, GL_TEXTURE2);
    
    //FINAL render
    _frame->add<4, render_commands::unbind_gbuffer>(GL_DRAW_FRAMEBUFFER);
    _frame->add<4, render_commands::bind_gbuffer>(&_gbuffer, GL_READ_FRAMEBUFFER);
    
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_left);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_right);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::bottom_left);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::bottom_right);
    /*
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::full_screen);
     */
}

void deferred_render_visitor::end_frame()
{
}