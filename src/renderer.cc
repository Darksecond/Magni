#include "renderer.h"

#include "game_object.h"
#include "resource_factory.h"
#include "program_resource_loader.h"
#include "Texture.h"
#include "scene.h"
#include "commands.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32
#include <iostream>

renderer::renderer(const glm::ivec2& screen_size)
: SCREEN_SIZE(screen_size), _texts()
{
}

void renderer::boot()
{
    //GLFW
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    
    if(!glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
    //GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    glGetError(); //ignore any errors coming from glewInit, because they can be safely ignored.
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    int max_color_attachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
    std::cout << "Max Color Attachments: " << max_color_attachments << std::endl;
    
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // GLFW settings
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);
    glfwSwapInterval(0); //uncomment for no v-sync
    
    //OpenGL settings
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);
    
    //resource loaders
    resource_factory::instance().add_loader("program", std::make_shared<program_resource_loader>());
    resource_factory::instance().add_loader("mesh", std::make_shared<default_resource_loader<Ymir::Mesh>>());
    resource_factory::instance().add_loader("texture", std::make_shared<default_resource_loader<Ymir::Texture>>());
    
    _deferred_renderer = std::unique_ptr<deferred_render_visitor>(new deferred_render_visitor(SCREEN_SIZE));
    
}

void renderer::build()
{
    _overlay_program = resource_factory::instance().resource<Ymir::Program>("overlay", "program");
    _holstein = resource_factory::instance().resource<Ymir::Texture>("Holstein.tga", "texture");
}

void renderer::shutdown()
{
    _deferred_renderer = nullptr;
    
    glfwCloseWindow();
    glfwTerminate();
}

bool renderer::step(std::shared_ptr<scene>& active_scene)
{
    if(!active_scene)
    {
        glfwSwapBuffers();
        return true;
    }
    
    auto world = active_scene->scene_graph_view();
    render_frame frame;
    
    _deferred_renderer->set_frame(&frame);
    
    _deferred_renderer->start_frame();
    world->accept(*_deferred_renderer);
    _deferred_renderer->end_frame();
    
    //TODO move to visitor?
    //TODO respond to messages to show text, edit text, remove text, etc
    frame.add<5, render_commands::bind_program>(_overlay_program);
    frame.add<5, render_commands::bind_texture>(_holstein, GL_TEXTURE0, "myTextureSampler");
    frame.add<5, render_commands::set_blend>(true, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    frame.add<5, render_commands::set_depth>(false, false);
    for(auto text : _texts)
    {
        frame.add<5, render_commands::draw_text>(text);
    }
    frame.add<5, render_commands::set_blend>(false);
    frame.add<5, render_commands::set_depth>(true, true);
    
    frame.execute();
    
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;
    
    glfwSwapBuffers();
    
    if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
        return false;
    
    return true;
}

void renderer::on_event(const event_t type, void* data)
{
    if (type == event_t::renderer_add_text)
    {
        text_event *te = (text_event*)data;
        _texts.push_back(te->_text);
    }
    else if(type == event_t::renderer_remove_text)
    {
        text_event *te = (text_event*)data;
        _texts.remove(te->_text);
    }
}
