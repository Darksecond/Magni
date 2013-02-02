#include "Mesh.h"

Mesh Mesh::cubeMesh()
{    
    std::vector<Vertex> vertices
    {
        //  X     Y     Z       U     V
        // bottom
        {-1.0f,-1.0f,-1.0f,   0.0f, 0.0f},
        { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f},
        {-1.0f,-1.0f, 1.0f,   0.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,   1.0f, 1.0f},
        {-1.0f,-1.0f, 1.0f,   0.0f, 1.0f},

        // top
        {-1.0f, 1.0f,-1.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,   1.0f, 0.0f},
        { 1.0f, 1.0f,-1.0f,   1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f, 1.0f,   1.0f, 1.0f},
        
        // front
        {-1.0f,-1.0f, 1.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f,   1.0f, 1.0f},
        { 1.0f,-1.0f, 1.0f,   0.0f, 0.0f},
        { 1.0f, 1.0f, 1.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f,   1.0f, 1.0f},
        
        // back
        {-1.0f,-1.0f,-1.0f,   0.0f, 0.0f},
        {-1.0f, 1.0f,-1.0f,   0.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f},
        { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f},
        {-1.0f, 1.0f,-1.0f,   0.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,   1.0f, 1.0f},
        
        // left
        {-1.0f,-1.0f, 1.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f,-1.0f,   1.0f, 0.0f},
        {-1.0f,-1.0f,-1.0f,   0.0f, 0.0f},
        {-1.0f,-1.0f, 1.0f,   0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f,   1.0f, 1.0f},
        {-1.0f, 1.0f,-1.0f,   1.0f, 0.0f},
        
        // right
        { 1.0f,-1.0f, 1.0f,   1.0f, 1.0f},
        { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f},
        { 1.0f, 1.0f,-1.0f,   0.0f, 0.0f},
        { 1.0f,-1.0f, 1.0f,   1.0f, 1.0f},
        { 1.0f, 1.0f,-1.0f,   0.0f, 0.0f},
        { 1.0f, 1.0f, 1.0f,   0.0f, 1.0f},
    };
    return Mesh(vertices);
}

Mesh::Mesh(const std::vector<Vertex>& vertices) : _vbo{0}, _vao{0}, _numVertices(vertices.size())
{
    //generate
    glGenBuffers(1, &_vbo);
    glGenVertexArrays(1, &_vao);

    //bind
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    
    //unbind
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) : _vbo(other._vbo), _vao(other._vao), _numVertices(other._numVertices)
{
    other._vao = 0;
    other._vbo = 0;
    other._numVertices = 0;
}

Mesh::~Mesh()
{
    if(_vao)
        glDeleteVertexArrays(1, &_vao);
    if(_vbo)
        glDeleteBuffers(1, &_vbo);
}

Mesh& Mesh::operator=(Mesh&& other)
{
    if(_vao)
        glDeleteVertexArrays(1, &_vao);
    if(_vbo)
        glDeleteBuffers(1, &_vbo);
    
    _vao = other._vao;
    _vbo = other._vbo;
    _numVertices = other._numVertices;
    
    other._vao = 0;
    other._vbo = 0;
    other._numVertices = 0;
    
    return *this;
}

void Mesh::draw(Program& gProgram) const
{
    //bind
    glBindVertexArray(_vao); //bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram.attrib("vert"));
    //glVertexAttribPointer(gProgram.attrib("vert"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, x));
    glVertexAttribPointer(gProgram.attrib("vert"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const GLvoid*>(offsetof(Vertex, x)));

    
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram.attrib("vertTexCoord"));
    glVertexAttribPointer(gProgram.attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex),
                          reinterpret_cast<const GLvoid*>(offsetof(Vertex, u)));
    
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_numVertices) );
    
    //unbind
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}