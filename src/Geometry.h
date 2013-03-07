#pragma once

#ifdef __APPLE__
    #include <GLEW/glew.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
#endif // _WIN32

#include <vector>

namespace Ymir
{
    typedef unsigned short Indice;

    struct Vertex
    {
        GLfloat x, y, z;
        GLfloat nx, ny, nz;
        GLfloat u, v;
    };

    /**
     * Geometry interface which can be used to construct a Mesh.
     */
    class Geometry
    {
    public:
        virtual const std::vector<Vertex>& vertices() const = 0;
        virtual const std::vector<Indice>& indices() const = 0;
    };
};
