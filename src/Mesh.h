#pragma once

#include <GLEW/glew.h>
#include <vector>

#include "Program.h"

struct Vertex
{
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat u, v;
};

class Mesh
{
    GLuint _vbo;
    GLuint _vao;
    GLuint _ibo; //indices buffer object
    size_t _numVertices;
    size_t _numIndices;
public:
    static Mesh cube();
    
    Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned short> indices);
    Mesh(Mesh&&);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&);
    
    void draw(Program& p) const;
    
    void bind() const;
    void unbind() const;
};