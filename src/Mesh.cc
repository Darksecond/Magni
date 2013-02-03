#include "Mesh.h"

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
    return Mesh(vertices);
}

Mesh::Mesh(const std::vector<Vertex>& vertices) : _vbo{0}, _vao{0}, _numVertices(vertices.size())
{
    //generate
    glGenBuffers(1, &_vbo);
    glGenVertexArrays(1, &_vao);

    bind();
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    
    unbind();
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
    
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_numVertices) );
    
    unbind();
}

void Mesh::bind() const
{
    glBindVertexArray(_vao); //bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
}

void Mesh::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}