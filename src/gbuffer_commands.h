#pragma once

#include "render_command.h"
#include "Program.h"
#include "render_frame.h"
#include "Mesh.h"
#include "material.h"
#include "gbuffer.h"

#ifdef __APPLE__
    #include <GLFW/GLFW.h>
    #include <GLEW/glew.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32
#include <memory>
#include <GLM/glm.hpp>
#include <iostream>

namespace render_commands
{
    class bind_gbuffer : public render_command
    {
    public:
        bind_gbuffer(gbuffer* buffer, GLenum target) : _gbuffer(buffer), _target(target) {}
        inline virtual void execute()
        {
            _gbuffer->bind(_target);
        }
    private:
        gbuffer* _gbuffer;
        GLenum _target;
    };
    
    class unbind_gbuffer : public render_command
    {
    public:
        unbind_gbuffer(GLenum target) : _target(target) {}
        inline virtual void execute()
        {
            glBindFramebuffer(_target, 0);
        }
    private:
        GLenum _target;
    };
    
    class set_read_buffer : public render_command
    {
    public:
        set_read_buffer(GLenum mode) : _mode(mode) {}
        set_read_buffer(gbuffer::GBUFFER_TEXTURE_TYPE type) : _mode(gbuffer::attachments[type].attachment) {}
        inline virtual void execute()
        {
            glReadBuffer(_mode);
        }
    private:
        GLenum _mode;
    };
    
    class set_draw_buffer : public render_command
    {
    public:
        set_draw_buffer(GLenum mode) : _mode(mode) {}
        set_draw_buffer(gbuffer::GBUFFER_TEXTURE_TYPE type) : _mode(gbuffer::attachments[type].attachment) {}
        inline virtual void execute()
        {
            glDrawBuffer(_mode);
        }
    private:
        GLenum _mode;
    };
    
    class set_draw_buffers : public render_command
    {
    public:
        set_draw_buffers(const GLsizei n, const gbuffer::GBUFFER_TEXTURE_TYPE* gbuffer_bufs) : _n(n)
        {
            _bufs = new GLenum[n];
            for(int i  = 0; i < n; ++i)
            {
                _bufs[i] = gbuffer::attachments[gbuffer_bufs[i]].attachment;
            }
        }
        
        ~set_draw_buffers() { delete [] _bufs;}
        inline virtual void execute()
        {
            glDrawBuffers(_n, _bufs);
        }
    private:
        GLenum* _bufs;
        const GLsizei _n;
    };
    
    class blit_fbo : public render_command
    {
    public:
        enum corner_t
        {
            top_left,
            top_right,
            bottom_left,
            bottom_right,
            full_screen,
        };
        
        blit_fbo(glm::ivec2 window_size, corner_t corner, GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_LINEAR) : _window_size(window_size), _corner(corner), _mask(mask), _filter(filter) {}
        inline virtual void execute()
        {
            GLint dstX0, dstY0, dstX1, dstY1;
            dstX0 = dstY0 = dstX1 = dstY1 = 0;
            if(_corner == bottom_left) //CHECK
            {
                dstX0 = 0;
                dstY0 = 0;
                dstX1 = _window_size.x / 2;
                dstY1 = _window_size.y / 2;
            }
            else if(_corner == top_right) //CHECK
            {
                dstX0 = _window_size.x / 2;
                dstY0 = _window_size.y / 2;
                dstX1 = _window_size.x;
                dstY1 = _window_size.y;
            }
            else if(_corner == top_left) //check
            {
                dstX0 = 0;
                dstY0 = _window_size.y / 2;
                dstX1 = _window_size.x / 2;
                dstY1 = _window_size.y;
            }
            else if(_corner == bottom_right) //CHECK
            {
                dstX0 = _window_size.x / 2;
                dstY0 = 0;
                dstX1 = _window_size.x;
                dstY1 = _window_size.y / 2;
            }
            else if(_corner == full_screen)
            {
                dstX0 = 0;
                dstY0 = 0;
                dstX1 = _window_size.x;
                dstY1 = _window_size.y;
            }
            glBlitFramebuffer(0, 0, _window_size.x, _window_size.y,
                              dstX0, dstY0, dstX1, dstY1,
                              _mask, _filter);
        }
    private:
        const glm::ivec2 _window_size;
        const corner_t _corner;
        const GLbitfield _mask;
        const GLenum _filter;
    };
    
    //TODO fetch gbuffer from state?
    class bind_gbuffer_texture : public render_command
    {
    public:
        bind_gbuffer_texture(gbuffer* gb, const char* uniform, const gbuffer::GBUFFER_TEXTURE_TYPE attachment, const GLenum tex_unit) : _gbuffer(gb), _uniform(uniform), _attachment(attachment), _texture_unit(tex_unit)
        {
        }
        inline virtual void execute()
        {
            _gbuffer->bind_texture(_texture_unit, _attachment);
            _frame->current_program()->setUniform(_uniform, (int)_texture_unit - GL_TEXTURE0);
        }
    private:
        gbuffer* _gbuffer;
        const char* _uniform;
        const gbuffer::GBUFFER_TEXTURE_TYPE _attachment;
        const GLenum _texture_unit;
    };
};