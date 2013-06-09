#pragma once

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/GLFW.h>
#endif // __APPLE__
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32
#include <GLM/glm.hpp>

class gbuffer
{
public:
    
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_NUM_TEXTURES
    };
    
    gbuffer(const glm::ivec2 WINDOW_SIZE);
    ~gbuffer();
    
    gbuffer(const gbuffer&) = delete;
    gbuffer(gbuffer&&) = delete;
    gbuffer& operator=(const gbuffer&) = delete;
    gbuffer& operator=(gbuffer&&) = delete;
private:
    GLuint _fbo;
    GLuint _textures[GBUFFER_NUM_TEXTURES];
    GLuint _depth_texture;
    GLuint _final_texture;
};