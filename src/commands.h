#pragma once

#include "render_command.h"
#include "Program.h"
#include "render_frame.h"
#include "Mesh.h"
#include "material.h"
#include "Texture.h"
#include "text.h"

#ifdef __APPLE__
    #include <GLFW/GLFW.h>
    #include <GLEW/glew.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32
#include <memory>
#include <GLM/glm.hpp>
#include <iostream>
#include <list>

namespace render_commands
{
    class bind_program : public render_command
    {
    public:
        bind_program(std::shared_ptr<Ymir::Program> p) : _program(p) {}
        inline virtual void execute()
        {
            _program->bind();
            _frame->set_current_program(_program);
        }
    private:
        std::shared_ptr<Ymir::Program> _program;
    };
    
    class unbind_program : public render_command
    {
    public:
        inline virtual void execute()
        {
            glUseProgram(0);
            _frame->set_current_program(nullptr);
        }
    };
    
    template <typename T>
    class set_uniform : public render_command
    {
    public:
        set_uniform(const char* name, T value) : _name(name), _value(value) {}
        inline virtual void execute()
        {
            _frame->current_program()->setUniform(_name, _value);
        }
    private:
        const char* _name;
        T _value;
    };
    
    class draw_model : public render_command
    {
    public:
        draw_model(std::shared_ptr<Ymir::Mesh> mesh, std::shared_ptr<material> mat, glm::mat4 matrix) : _mesh(mesh), _material(mat), _matrix(matrix)
        {}
        inline virtual void execute()
        {
            if(_material)
                _material->bind(*_frame->current_program());
            _frame->current_program()->setUniform("model", _matrix);
            _mesh->draw(*_frame->current_program());
        }
    private:
        std::shared_ptr<Ymir::Mesh> _mesh;
        std::shared_ptr<material> _material;
        glm::mat4 _matrix;
    };
    
    class clear : public render_command
    {
    public:
        clear(GLbitfield mask) : _mask(mask) {}
        inline virtual void execute()
        {
            glClear(_mask);
        }
    private:
        GLbitfield _mask;
    };
    
    class clear_color : public render_command
    {
    public:
        clear_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) : _r(r), _g(g), _b(b), _a(a) {}
        inline virtual void execute()
        {
            glClearColor(_r, _g, _b, _a);
        }
    private:
        GLfloat _r, _g, _b, _a;
    };
    
    class set_depth : public render_command
    {
    public:
        set_depth(bool test_enabled, bool mask, GLenum func = GL_LESS) : _test_enabled(test_enabled), _mask(mask), _func(func) {}
        inline virtual void execute()
        {
            if(_test_enabled)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(_func);
                glDepthMask(_mask);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }
    private:
        bool _test_enabled;
        bool _mask;
        GLenum _func;
    };
    
    class set_blend : public render_command
    {
    public:
        set_blend(bool enable, GLenum equation = GL_FUNC_ADD, GLenum sfactor = GL_ONE, GLenum dfactor = GL_ZERO) : _enabled(enable), _equation(equation), _func_sfactor(sfactor), _func_dfactor(dfactor) {}
        inline virtual void execute()
        {
            if(_enabled)
            {
                glEnable(GL_BLEND);
                glBlendFunc(_func_sfactor, _func_dfactor);
                glBlendEquation(_equation);
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }
    private:
        bool _enabled;
        GLenum _equation;
        GLenum _func_sfactor;
        GLenum _func_dfactor;
    };
    
    class set_culling : public render_command
    {
    public:
        set_culling(bool cull, GLenum face = GL_BACK) : _cull(cull), _face(face) {}
        inline virtual void execute()
        {
            if(_cull)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(_face);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }
    private:
        bool _cull;
        GLenum _face;
    };
    
    class set_stencil_state : public render_command
    {
    public:
        set_stencil_state(bool enable) : _enable(enable) {}
        inline virtual void execute()
        {
            if(_enable)
            {
                glEnable(GL_STENCIL_TEST);
            }
            else
            {
                glDisable(GL_STENCIL_TEST);
            }
        }
    private:
        bool _enable;
    };
    
    class set_stencil_func : public render_command
    {
    public:
        set_stencil_func(GLenum func, GLint ref, GLuint mask) : _func(func), _ref(ref), _mask(mask) {}
        inline virtual void execute()
        {
            glStencilFunc(_func, _ref, _mask);
        }
    private:
        GLenum _func;
        GLint _ref;
        GLuint _mask;
    };
    
    class set_stencil_op : public render_command
    {
    public:
        set_stencil_op(GLenum sfail, GLenum dpfail, GLenum dppass, GLenum face = GL_FRONT_AND_BACK) : _face(face), _sfail(sfail), _dpfail(dpfail), _dppass(dppass) {}
        inline virtual void execute()
        {
            glStencilOpSeparate(_face, _sfail, _dpfail, _dppass);
        }
    private:
        GLenum _face, _sfail, _dpfail, _dppass;
    };
    
    class group : public render_command
    {
    public:
        group() : _commands() {}
        
        template <typename T, typename... Args>
        inline void add(Args... args)
        {
            _commands.push_back(std::make_shared<T>(args...));
        }
        
        inline virtual void execute()
        {
            for(auto command : _commands)
            {
                command->set_frame(_frame);
                command->execute();
            }
        }
    private:
        std::list<std::shared_ptr<render_command>> _commands;
    };
    
    class execute_queue : public render_command
    {
    public:
        execute_queue(int queue) : _queue(queue) {}
        inline virtual void execute()
        {
            _frame->execute_queue(_queue);
        }
    private:
        int _queue;
    };
    
    class bind_texture : public render_command
    {
    public:
        bind_texture(std::shared_ptr<Ymir::Texture> tex, const GLenum unit, const char* name) : _texture(tex), _unit(unit), _name(name) {}
        inline virtual void execute()
        {
            _texture->bind(_unit);
            _frame->current_program()->setUniform(_name, (int)_unit-GL_TEXTURE0);
        }
    private:
        std::shared_ptr<Ymir::Texture> _texture;
        GLenum _unit;
        const char* _name;
    };
    
    class draw_text : public render_command
    {
    public:
        draw_text(std::shared_ptr<text> text) : _text(text) {}
        inline virtual void execute()
        {
            _text->draw(*_frame->current_program());
        }
    private:
        std::shared_ptr<text> _text;
    };
};