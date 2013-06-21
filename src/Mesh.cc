#include "Mesh.h"

#include "CubeGeometry.h"

#include <stdexcept>
#include <iostream>

using namespace Ymir;

Mesh Mesh::cube()
{
    CubeGeometry cube;
    return Mesh{cube};
}

Mesh::Mesh(Geometry& geometry) : _vbo{0}, _vao{0}
{
    if(geometry.vertices().size() == 0)
        throw std::runtime_error("no vertices in geometry");
    if(geometry.indices().size() == 0)
        throw std::runtime_error("no indices in geometry");
    
    calculate_aabb(geometry);
    
    _numVertices = geometry.vertices().size();
    _numIndices = geometry.indices().size();

    //generate
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    glGenVertexArrays(1, &_vao);

    bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _numVertices,
                 &geometry.vertices().front(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indice) * _numIndices,
                 &geometry.indices().front(), GL_STATIC_DRAW);

    unbind();
}

Mesh::Mesh(Mesh&& other) : _vbo(other._vbo), _vao(other._vao), _numVertices(other._numVertices), _numIndices(other._numIndices), _ibo(other._ibo), _aabb(other._aabb)
{
    other._vao = 0;
    other._vbo = 0;
    other._ibo = 0;
    other._numVertices = 0;
    other._numIndices = 0;
}

Mesh::~Mesh()
{
    if(_vao)
        glDeleteVertexArrays(1, &_vao);
    if(_vbo)
        glDeleteBuffers(1, &_vbo);
    if(_ibo)
        glDeleteBuffers(1, &_ibo);
}

Mesh& Mesh::operator=(Mesh&& other)
{
    if(_vao)
        glDeleteVertexArrays(1, &_vao);
    if(_vbo)
        glDeleteBuffers(1, &_vbo);
    if(_ibo)
        glDeleteBuffers(1, &_ibo);

    _vao = other._vao;
    _vbo = other._vbo;
    _ibo = other._ibo;
    _numVertices = other._numVertices;
    _numIndices = other._numIndices;

    other._vao = 0;
    other._vbo = 0;
    other._ibo = 0;
    other._numVertices = 0;
    other._numIndices = 0;

    return *this;
}

void Mesh::draw(Program& gProgram) const
{
    bind();

    //TODO use explicit bound indexes

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram.attrib("vert"));
    glVertexAttribPointer(gProgram.attrib("vert"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const GLvoid*>(offsetof(Vertex, x)));

    // connect the nxnynz to the "vertNormal" attribute of the vertex shader
    try
    {
        glEnableVertexAttribArray(gProgram.attrib("vertNormal"));
        glVertexAttribPointer(gProgram.attrib("vertNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const GLvoid*>(offsetof(Vertex, nx)));


    }
    catch(std::runtime_error e)
    {
        //do nothing, program does not use vertNormals
    }
    
    try
    {
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram.attrib("vertTexCoord"));
    glVertexAttribPointer(gProgram.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex),
                          reinterpret_cast<const GLvoid*>(offsetof(Vertex, u)));
    }
    catch(std::runtime_error e)
    {
        //do nothing
    }


    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_numIndices), GL_UNSIGNED_SHORT,
                   reinterpret_cast<const GLvoid*>(0));

    unbind();
}

void Mesh::bind() const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
}

void Mesh::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Mesh::calculate_aabb(const Geometry& geo)
{
    if(geo.vertices().empty())
    {
        _aabb.reset(glm::vec3(0));
    }
    else
    {
        glm::vec3 first_point(geo.vertices()[0].x, geo.vertices()[0].y, geo.vertices()[0].z);
        _aabb.reset(first_point);
        for(int i = 1; i < geo.vertices().size(); ++i)
        {
            glm::vec3 point(geo.vertices()[i].x, geo.vertices()[i].y, geo.vertices()[i].z);
            _aabb.add_point(point);
        }
    }
}
