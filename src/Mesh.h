#pragma once

#include "Program.h"
#include "Geometry.h"

#include <GLEW/glew.h>
#include <vector>

namespace Ymir
{
    class Mesh
    {
        GLuint _vbo;
        GLuint _vao;
        GLuint _ibo; //indices buffer object
        size_t _numVertices;
        size_t _numIndices;
    public:
        static Mesh cube();
        
        Mesh(Geometry& geometry);
        Mesh(Mesh&&);
        ~Mesh();
        Mesh(const Mesh&) = delete;
        
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&);
        
        void draw(Program& p) const;
        
        void bind() const;
        void unbind() const;
    };
};