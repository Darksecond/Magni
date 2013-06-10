#include "gbuffer.h"

#include <stdexcept>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

const gbuffer::attachment gbuffer::attachments[] = {
    {GL_RGB32F, GL_RGB, GL_COLOR_ATTACHMENT0},
    {GL_RGB32F, GL_RGB, GL_COLOR_ATTACHMENT1},
    {GL_RGB32F, GL_RGB, GL_COLOR_ATTACHMENT2},
    {GL_DEPTH32F_STENCIL8, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL_ATTACHMENT},
    {GL_RGBA, GL_RGB, GL_COLOR_ATTACHMENT3},
};

gbuffer::gbuffer(const glm::ivec2 WINDOW_SIZE)
{
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(_textures), _textures);
    
    for(unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(_textures); ++i)
    {
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, attachments[i].internal_format, WINDOW_SIZE.x, WINDOW_SIZE.y, 0, attachments[i].format, GL_FLOAT, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachments[i].attachment, GL_TEXTURE_2D, _textures[i], 0);
    }
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("framebuffer error");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

gbuffer::~gbuffer()
{
    glDeleteFramebuffers(1, &_fbo);
    glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(_textures), _textures);
}

void gbuffer::bind(GLenum target)
{
    glBindFramebuffer(target, _fbo);
}
