#include "text.h"
#include <iostream>

text::text(const glm::vec2& pos, const std::string& text, const int size) : _text(text), _size(size)
{
    _bottom_left = pos;
    _top_right = glm::vec2(pos.x + (size * text.size() - 1),
                           pos.y + size);
    
    _vao = _vbo = _uv_vbo = 0;
    generate();
}

text::~text()
{
    if(_vao)
        glDeleteVertexArrays(1, &_vao);
    if(_vbo)
        glDeleteBuffers(1, &_vbo);
    if(_uv_vbo)
        glDeleteBuffers(1, &_uv_vbo);
}

void text::draw(Ymir::Program& p) const
{
    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glEnableVertexAttribArray(p.attrib("vertexPosition_screenspace"));
    glVertexAttribPointer(p.attrib("vertexPosition_screenspace"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _uv_vbo);
    glEnableVertexAttribArray(p.attrib("vertexUV"));
    glVertexAttribPointer(p.attrib("vertexUV"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_verts_count);
    
    glBindVertexArray(0);
}

void text::generate()
{
    if(_vao)
    {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    if(_vbo)
    {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
    if(_uv_vbo)
    {
        glDeleteBuffers(1, &_uv_vbo);
        _uv_vbo = 0;
    }
    
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_uv_vbo);
    glGenVertexArrays(1, &_vao);
    
    std::vector<glm::vec2> verts;
    std::vector<glm::vec2> uvs;
    
    for(int i = 0; i < _text.size(); ++i)
    {
        glm::vec2 up_left(_bottom_left.x + i * _size,
                          _bottom_left.y + _size);
        
        glm::vec2 up_right(_bottom_left.x + i * _size + _size,
                           _bottom_left.y + _size);
        
        glm::vec2 down_right(_bottom_left.x + i * _size + _size,
                             _bottom_left.y);
        
        glm::vec2 down_left(_bottom_left.x + i * _size,
                            _bottom_left.y);
        
        verts.push_back(up_left);
        verts.push_back(down_left);
        verts.push_back(up_right);
        
        verts.push_back(down_right);
        verts.push_back(up_right);
        verts.push_back(down_left);
        
        char character = _text[i];
        float uv_x = (character%16)/16.0f;
        float uv_y = (character/16)/16.0f;
        
        glm::vec2 uv_up_left   (uv_x           , uv_y);
        glm::vec2 uv_up_right  (uv_x+1.0f/16.0f, uv_y);
        glm::vec2 uv_down_right(uv_x+1.0f/16.0f, (uv_y+1.0f/16.0f));
        glm::vec2 uv_down_left (uv_x           , (uv_y+1.0f/16.0f));
        
        uvs.push_back(uv_up_left);
        uvs.push_back(uv_down_left);
        uvs.push_back(uv_up_right);
        
        uvs.push_back(uv_down_right);
        uvs.push_back(uv_up_right);
        uvs.push_back(uv_down_left);
    }
    
    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2), &verts[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, _uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    
    _verts_count = verts.size();
    
    glBindVertexArray(0);
}