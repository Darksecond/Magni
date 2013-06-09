#pragma once

#include "render_command.h"
#include "Program.h"
#include "render_frame.h"
#include "Mesh.h"
#include "material.h"

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
            _material->bind();
            _frame->current_program()->setUniform("tex", 0); //diffuse == 0 //TODO magic numbers
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
};