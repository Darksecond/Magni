#pragma once

#include <GLEW/glew.h>
#include <vector>

#include "Program.h"

struct Vertex
{
    GLfloat x, y, z;
    GLfloat u, v;
};

class Mesh
{
    GLuint _vbo;
    GLuint _vao;
    size_t _numVertices;
public:
    static Mesh cubeMesh();
    
    Mesh(const std::vector<Vertex>& vertices);
    Mesh(Mesh&&);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&);
    
    void draw(Program& p) const;
};