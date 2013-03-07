#pragma once

#include "Program.h"
#include "Geometry.h"

//TODO
#include "StreamReader.h"
#include "ObjGeometry.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
#endif // _WIN32

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
