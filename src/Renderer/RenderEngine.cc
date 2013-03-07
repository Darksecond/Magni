#include "RenderEngine.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <sstream>

#include "Light.h"
#include "LightComponent.h"
#include "SpatialComponent.h"
#include "Text.h"

using namespace Ymir;

const glm::vec2 SCREEN_SIZE(800, 600);

int frames = 0;
double t0 = 0.0;
void showFPS() {
    
    double t, fps;
    
    // Get current time
    t = glfwGetTime();  // Get number of seconds since glfwInit()
    // Calculate and display FPS (frames per second) in title bar.
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        std::stringstream ss;
        ss << "Ymir | FPS: " << fps;
        glfwSetWindowTitle(ss.str().c_str());
        t0 = t;
        frames = 0;
    }
    frames ++;
}

RenderEngine::RenderEngine(ResourceManager<Program,
                           ProgramResourceLoader>& programManager,
                           ResourceManager<Texture>& textureManager,
                           ResourceManager<Cubemap, CubemapResourceLoader>& cubemapManager
                           ) : lights{}, models{}, texts{}
{
    initGLFW();
    initGLEW();
    initOpenGL();
    
    texture_program = programManager.resource("texture");
    phong_program = programManager.resource("phong");
    overlay_program = programManager.resource("overlay");
    holstein = textureManager.resource("Holstein.tga");
    
    sky = cubemapManager.resource("sky.jpg");
    sky_program = programManager.resource("sky");
}

void RenderEngine::initGLFW()
{
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    
    if(!glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);
    //glfwSwapInterval(0); //uncomment for no v-sync
}

void RenderEngine::initGLEW()
{
    glewExperimental = GL_TRUE; //stops glew crashing on OSX
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    glGetError(); //ignore any errors coming from glewInit, because they can be safely ignored.
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
}

void RenderEngine::initOpenGL()
{
    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

RenderEngine::~RenderEngine()
{
    glfwTerminate();
}

//TODO not really used anymore since Scene was introduced
void RenderEngine::registerEntity(Entity& entity)
{    
    lights.registerEntity(entity);
    models.registerEntity(entity);
    
    std::unique_ptr<Camera> cameraNode = Camera::fromEntity(entity);
    if(cameraNode)
    {
        _camera = std::move(cameraNode);
    }
}

void RenderEngine::unregisterEntity(Entity& entity)
{    
    lights.unregisterEntity(entity);
    models.unregisterEntity(entity);
    //camera
}

void RenderEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    lights.addComponent(entity, component_type);
    models.addComponent(entity, component_type);
    
    //camera
    if(component_type == CameraComponent::type() || component_type == SpatialComponent::type())
    {
        std::unique_ptr<Camera> cameraNode = Camera::fromEntity(entity);
        if(cameraNode && _camera == nullptr)
        {
            _camera = std::move(cameraNode);
        }
    }
}

void renderTexture(Program& p, const Texture& t, const Camera& c, const Mesh& m, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //material
    // bind the texture and set the "tex" uniform in the fragment shader
    TextureContext tex{t, GL_TEXTURE0};
    p.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    
    //always
    m.draw(p);
}

void renderLight(Program& p, const Camera& c, const Mesh& m, const Light& l, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(modelView))); //not part of light, but only needed for lights so far
    
    //material    
    model.attach(p);
    
    //light
    l.attach(p, c);
    
    p.setUniform("normal", normal); //not part of light, but only needed for lights so far
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //always
    m.draw(p);
}

void render(Program& pt, Program& pl, const Camera& c,
            const NodeCache<Light>& lights, Model& model)
{
    //static settings, really
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    //transparency fix
    //http://www.opengl.org/discussion_boards/showthread.php/145630-Translucency-and-multi-pass-lighting
    bool first = true;
    for(auto& light : lights)
    {
        if(first)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            first = false;
        }
        else
        {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        }
        renderLight(pl, c, *model.model.mesh, *light, model);
    }
    
    glBlendFunc(GL_ZERO,GL_SRC_COLOR);
    renderTexture(pt, *model.model.texture, c, *model.model.mesh, model);
    
    glDisable(GL_BLEND);
}

void renderOverlay(Program& p, Texture& holstein, Text& text)
{    
    std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
    
    for ( unsigned int i=0 ; i < text.text.size(); i++ )
    {
        glm::vec2 vertex_up_left    = glm::vec2( text.position.x+i*text.size            , text.position.y+text.size );
		glm::vec2 vertex_up_right   = glm::vec2( text.position.x+i*text.size + text.size, text.position.y+text.size );
		glm::vec2 vertex_down_right = glm::vec2( text.position.x+i*text.size + text.size, text.position.y           );
		glm::vec2 vertex_down_left  = glm::vec2( text.position.x+i*text.size            , text.position.y           );
        
		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );
        
		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);
        
		char character = text.text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;
        
		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );
        
		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
    }
    
    ProgramContext pc{p};
    
    //create VBO's, VAO
    GLuint vertex_VBO, UV_VBO, VAO;
    
    glGenBuffers(1, &vertex_VBO);
    glGenBuffers(1, &UV_VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(p.attrib("vertexPosition_screenspace"));
    glVertexAttribPointer(p.attrib("vertexPosition_screenspace"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(p.attrib("vertexUV"));
	glVertexAttribPointer(p.attrib("vertexUV"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    TextureContext h{holstein, GL_TEXTURE0};
    p.setUniform("myTextureSampler", 0);
    
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() );
    
	glDisable(GL_BLEND);
    
	glDisableVertexAttribArray(p.attrib("vertexPosition_screenspace"));
	glDisableVertexAttribArray(p.attrib("vertexUV"));
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertex_VBO);
    glDeleteBuffers(1, &UV_VBO);
}

void renderSkybox(Cubemap& sky, Program& p, Camera& c)
{
    static const GLfloat cube_vertices[] = {
        -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
    };
    
    static const GLushort cube_indices[] = {
        //0, 1, 2, 3,
        //3, 2, 6, 7,
        //7, 6, 5, 4,
        //4, 5, 1, 0,
        //0, 3, 7, 4,
        //1, 2, 6, 5,
        
        0, 1, 2,
        0, 2, 3,
        
        3, 2, 6,
        3, 6, 7,
        
        7, 6, 5,
        7, 5, 4,
        
        4, 5, 1,
        4, 1, 0,
        
        0, 3, 7,
        0, 7, 4,
        
        1, 2, 6,
        1, 6, 5,
    };
    
    ProgramContext pc{p};
    
    GLuint vao, vbo, ibo;
    
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &vao);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(p.attrib("vertex"));
    glVertexAttribPointer(p.attrib("vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glm::mat4 M = c.projectionMatrix() * c.viewMatrix() * glm::scale(glm::translate(glm::mat4{}, -c._spatial.globalPosition()), glm::vec3{50});
    
    p.setUniform("PVM", M);

    sky.bind(GL_TEXTURE0);
    p.setUniform("cubemap", 0);
    
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glEnable(GL_CULL_FACE);
    sky.unbind();
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    
}

void RenderEngine::update(int pass, double delta)
{
    if(pass == 1)
    {
        glClearColor(0.0, 0.0, 0.0, 1); // black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderSkybox(*sky, *sky_program, *_camera);
        
        for(auto& model : models)
        {
            render(*texture_program, *phong_program, *_camera, lights, *model);
        }
        
        for(auto t : texts)
        {
            renderOverlay(*overlay_program, *holstein, *t);
        }
        
        showFPS();
        glfwSwapBuffers();
    }
}