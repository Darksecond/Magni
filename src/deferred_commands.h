#pragma once

#include "render_command.h"
#include "Program.h"
#include "render_frame.h"
#include "Mesh.h"
#include "gbuffer.h"
#include "spatial.h"
#include "light.h"

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
    class draw_light : public render_command
    {
    public:
        draw_light(std::shared_ptr<Ymir::Mesh> mesh, spatial spatial, float radius) : _mesh(mesh), _spatial(spatial), _radius(radius) {}
        virtual inline void execute()
        {
            _frame->current_program()->setUniform("model", _spatial.matrix());
            _frame->current_program()->setUniform("light.position", _spatial.translation());
            
            _frame->current_program()->setUniform("light.radius", _radius);
            
            _mesh->draw(*_frame->current_program());
        }
    private:
        std::shared_ptr<Ymir::Mesh> _mesh;
        spatial _spatial;
        float _radius;
    };
};