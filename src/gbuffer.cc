#include "gbuffer.h"

#include <stdexcept>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

gbuffer::gbuffer(const glm::ivec2 WINDOW_SIZE)
{
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    
    //create gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(_textures), _textures);
    glGenTextures(1, &_depth_texture);
    glGenTextures(1, &_final_texture);
    
    for(unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(_textures); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WINDOW_SIZE.x, WINDOW_SIZE.y, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
    }
    
    //depth
    glBindTexture(GL_TEXTURE_2D, _depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WINDOW_SIZE.x, WINDOW_SIZE.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);
    
    //final
    glBindTexture(GL_TEXTURE_2D, _final_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_SIZE.x, WINDOW_SIZE.y, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, _final_texture, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("framebuffer error");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

gbuffer::~gbuffer()
{
    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(_textures), _textures);
    glDeleteTextures(1, &_depth_texture);
    glDeleteTextures(1, &_final_texture);
}