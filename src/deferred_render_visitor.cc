#include "deferred_render_visitor.h"

#include "commands.h"
#include "render_frame.h"
#include "gbuffer_commands.h"
#include "deferred_commands.h"
#include "camera.h"
#include "model.h"
#include "light.h"
#include "resource_factory.h"
#include "Program.h"
#include "Mesh.h"

deferred_render_visitor::deferred_render_visitor(glm::ivec2 SCREEN_SIZE) :
_gbuffer(SCREEN_SIZE),
_SCREEN_SIZE(SCREEN_SIZE),
_geometry_program(resource_factory::instance().resource<Ymir::Program>("geometry", "program")),
_point_light_program(resource_factory::instance().resource<Ymir::Program>("point_lighting", "program")),
_sphere_mesh(resource_factory::instance().resource<Ymir::Mesh>("sphere.obj", "mesh")),
_null_program(resource_factory::instance().resource<Ymir::Program>("null", "program"))
{
}

deferred_render_visitor::~deferred_render_visitor()
{
}


void deferred_render_visitor::start_visit(light& l)
{
    auto group = std::make_shared<render_commands::group>();
    
    //stencil pass
    group->add<render_commands::set_draw_buffer>(GL_NONE);
    group->add<render_commands::bind_program>(_null_program);
    group->add<render_commands::set_depth>(true, false);
    group->add<render_commands::set_culling>(false);
    group->add<render_commands::set_blend>(false);
    group->add<render_commands::clear>(GL_STENCIL_BUFFER_BIT);
    group->add<render_commands::set_stencil_func>(GL_ALWAYS, 0, 0);
    group->add<render_commands::set_stencil_op>(GL_KEEP, GL_INCR, GL_KEEP, GL_BACK);
    group->add<render_commands::set_stencil_op>(GL_KEEP, GL_DECR, GL_KEEP, GL_FRONT);
    
    group->add<render_commands::execute_queue>(31); //camera
    
    spatial s = l.global();
    s.scale(glm::vec3(l.radius(), l.radius(), l.radius()));
    group->add<render_commands::draw_model>(_sphere_mesh, nullptr, s.matrix());
    //end stencil pass
    
    //light pass
    group->add<render_commands::set_draw_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    group->add<render_commands::bind_program>(_point_light_program);
    group->add<render_commands::bind_gbuffer_texture>(&_gbuffer, "g_diffuse", gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE, GL_TEXTURE0);
    group->add<render_commands::bind_gbuffer_texture>(&_gbuffer, "g_position", gbuffer::GBUFFER_TEXTURE_TYPE_POSITION, GL_TEXTURE1);
    group->add<render_commands::bind_gbuffer_texture>(&_gbuffer, "g_normal", gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL, GL_TEXTURE2);
    
    group->add<render_commands::set_stencil_func>(GL_NOTEQUAL, 0, 0xFF);
    group->add<render_commands::set_depth>(false, false);
    group->add<render_commands::set_blend>(true, GL_FUNC_ADD, GL_ONE, GL_ONE);
    
    group->add<render_commands::set_culling>(true, GL_FRONT);
    
    group->add<render_commands::set_uniform<glm::vec2>>("screen_size", (glm::vec2)_SCREEN_SIZE);
    group->add<render_commands::execute_queue>(31); //camera
    
    group->add<render_commands::draw_light>(_sphere_mesh, l.global(), l.radius());
    //end light pass
    
    _frame->add(3, group);
}

void deferred_render_visitor::start_visit(camera& c)
{
    //geometry pass
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("projection", c.projection());
    _frame->add<0, render_commands::set_uniform<glm::mat4>>("view", glm::inverse(c.global().matrix()));
    //lighting pass
    _frame->add<31, render_commands::set_uniform<glm::mat4>>("view", glm::inverse(c.global().matrix()));
    _frame->add<31, render_commands::set_uniform<glm::mat4>>("projection", c.projection());
}

void deferred_render_visitor::start_visit(model& m)
{
    _frame->add<1, render_commands::draw_model>(m.mesh(), m.material(), m.global().matrix());
}

void deferred_render_visitor::start_frame()
{
    _frame->ignore(31);
    //prepare for geometry pass
    _frame->add<0, render_commands::bind_program>(_geometry_program);
    _frame->add<0, render_commands::unbind_gbuffer>(GL_READ_FRAMEBUFFER);
    _frame->add<0, render_commands::bind_gbuffer>(&_gbuffer, GL_DRAW_FRAMEBUFFER);
    //clear final buffer
    _frame->add<0, render_commands::set_draw_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT);
    
    gbuffer::GBUFFER_TEXTURE_TYPE bufs[] = {
        gbuffer::GBUFFER_TEXTURE_TYPE_POSITION,
        gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE,
        gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL
    };
    _frame->add<0, render_commands::set_draw_buffers>(3, bufs);
    _frame->add<0, render_commands::clear_color>(0, 0, 0, 1);
    _frame->add<0, render_commands::set_depth>(true, true);
    _frame->add<0, render_commands::set_blend>(false);
    _frame->add<0, render_commands::set_culling>(true);
    _frame->add<0, render_commands::clear>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    _frame->add<2, render_commands::set_stencil_state>(true);
    
    //FINAL render
    _frame->add<4, render_commands::set_stencil_state>(false);
    _frame->add<4, render_commands::unbind_gbuffer>(GL_DRAW_FRAMEBUFFER);
    _frame->add<4, render_commands::bind_gbuffer>(&_gbuffer, GL_READ_FRAMEBUFFER);
    
    /*
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_left);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::top_right);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::bottom_left);
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::bottom_right);
    */
    _frame->add<4, render_commands::set_read_buffer>(gbuffer::GBUFFER_TEXTURE_TYPE_FINAL);
    _frame->add<4, render_commands::blit_fbo>(_SCREEN_SIZE, render_commands::blit_fbo::full_screen);
}

void deferred_render_visitor::end_frame()
{
}