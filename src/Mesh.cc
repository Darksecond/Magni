#include "Mesh.h"

using namespace Ymir;

Mesh Mesh::cube()
{    
    std::vector<Vertex> vertices
    {
        //  X     Y     Z       NX     NY     NZ       U     V
        // bottom
        {-1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f},
        { 1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
        {-1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f},
        {-1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f},

        // top
        {-1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
        { 1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f},
        
        // front
        {-1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f},
        { 1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f},
        { 1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f},
        
        // back
        {-1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f},
        {-1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f},
        
        // left
        {-1.0f,-1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
        {-1.0f,-1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
        {-1.0f,-1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
        {-1.0f, 1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
        
        // right
        { 1.0f,-1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
        { 1.0f, 1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
        { 1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
    };
    std::vector<unsigned short> indices{};
    return Mesh(vertices, indices);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned short> indices) : _vbo{0}, _vao{0}, _numVertices(vertices.size())
{
    //generate indices if they are unavailable
    if(indices.size() == 0)
    {
        indices.reserve(vertices.size()); //reserve the amount of indices needed, to speed generation up
        for(int i = 0; i < vertices.size(); i++)
            indices.push_back(i);
    }
    
    _numIndices = indices.size();
    
    //generate
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    glGenVertexArrays(1, &_vao);

    bind();
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices.front(), GL_STATIC_DRAW);
    
    unbind();
}

Mesh::Mesh(Mesh&& other) : _vbo(other._vbo), _vao(other._vao), _numVertices(other._numVertices), _numIndices(other._numIndices), _ibo(other._ibo)
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
    
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram.attrib("vertTexCoord"));
    glVertexAttribPointer(gProgram.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex),
                          reinterpret_cast<const GLvoid*>(offsetof(Vertex, u)));
    
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