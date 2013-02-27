#pragma once

#include "Program.h"
#include "Geometry.h"

#include <GLEW/glew.h>
#include <vector>

//TODO
#include "StreamReader.h"
#include "ObjGeometry.h"

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
        static Mesh fromStream(StreamReader& stream)
        {
            ObjGeometry geom{stream.stream()};
            return Mesh{geom};
        }
        
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