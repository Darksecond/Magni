#pragma once

#include "render_command.h"
#include "Program.h"
#include "render_frame.h"
#include "Mesh.h"
#include "material.h"

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
};