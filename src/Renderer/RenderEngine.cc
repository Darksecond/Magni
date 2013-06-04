#include "RenderEngine.h"

#include "Light.h"
#include "../LightComponent.h"
#include "../SpatialComponent.h"
#include "../SizeComponent.h"
#include "Text.h"
#include "HUDElementVisitor.h"
#include "HUDElement.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/glfw.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <math.h>

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
                           ResourceManager<Cubemap, CubemapResourceLoader>& cubemapManager,
                           ResourceManager<Mesh>& meshManager
                           ) : lights{}, models{}, texts{}, grid(false)
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

    // grid refactor nomination
    grid_program = programManager.resource("grid");
    // end refactor nomination

    _grass = textureManager.resource("grass.png");
    _mountain = textureManager.resource("mountain.png");
    _water = textureManager.resource("water.png");
    _square = meshManager.resource("track.obj");

    _selection_selecting = false;
    _selection_end = _selection_start = glm::vec3(0);
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
    //glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);
    glfwSwapInterval(1); //uncomment for no v-sync
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

void renderTexture(Program& p, const Texture& t, const Camera& c, const Mesh& m, const SpatialComponent& model_spatial)
{
    //always
    //p is part of material!
    ProgramContext pc{p};

    //always
    glm::mat4 view = c.viewMatrix();
    //glm::mat4 modelView = view * model.spatial.matrix();
    glm::mat4 modelView = view * model_spatial.matrix();

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
    renderTexture(pt, *model.model.texture, c, *model.model.mesh, model.spatial);

    glDisable(GL_BLEND);
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

    glm::mat4 M = c.projectionMatrix() * c.viewMatrix() * glm::scale(glm::translate(glm::mat4{}, c._spatial.globalPosition()), glm::vec3{50});

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

void renderTileMap(TileMap& tilemap, Program& pt, Program& pl, const Camera& c,
                   const NodeCache<Light>& lights, std::shared_ptr<Texture>& grass, std::shared_ptr<Texture>& water, std::shared_ptr<Texture>& mountain, std::shared_ptr<Mesh>& square)
{
    //needs mesh for square
    //needs textures for the things
    for(int i = 0; i < tilemap.getMapHeight(); ++i)
    {
        for(int j = 0; j < tilemap.getMapWidth(); ++j)
        {
            SpatialComponent s(glm::vec3(i-9.5, 0, j-9.5));
            s.parent = nullptr;
            s.scale = glm::vec3(1.0/tilemap.getMapHeight());

            Tile::Type tile_type = tilemap.getType(i, j);
            std::shared_ptr<Texture> tex = grass;
            if(tile_type == Tile::Type::WATER)
                tex = water;
            if(tile_type == Tile::Type::MOUNTAIN)
                tex = mountain;

            renderTexture(pt, *tex, c, *square, s);

            //ModelComponent mc(square, tex);
            //Model model(mc, s);
            //render(pt, pl, c, lights, model);
        }
    }
}

void drawSelection(glm::vec3 start, glm::vec3 end, const Camera& camera, Program& program)
{
    //TODO color (white) instead of red

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    ProgramContext pc {program};

    glm::mat4 projection = camera.projectionMatrix();
    glm::mat4 view = camera.viewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = projection * view * model;

    start.y = start.y + 0.01f;
    end.y   = end.y + 0.01f;

    int bufferSize = 8*3;
    glm::vec3 buffer[8];
    buffer[0] = glm::vec3(start.x, start.y, start.z);
    buffer[1] = glm::vec3(start.x, end.y, end.z);

    buffer[2] = glm::vec3(end.x, end.y, start.z);
    buffer[3] = glm::vec3(start.x, start.y, start.z);

    buffer[4] = glm::vec3(end.x, end.y, start.z);
    buffer[5] = glm::vec3(end.x, end.y, end.z);

    buffer[6] = glm::vec3(start.x, end.y, end.z);
    buffer[7] = glm::vec3(end.x, end.y, end.z);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(GLfloat), &buffer, GL_STATIC_DRAW);

    program.setUniform("MVP", MVP);
    program.setUniform("inputColor", glm::vec3(1, 1, 1));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glVertexAttribPointer(
                          0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0
                          );

    glDrawArrays(GL_LINES, 0, bufferSize / 3);

    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
}

void RenderEngine::update(int pass, double delta)
{
    if(pass == 1)
    {
        glClearColor(0.0, 0.0, 0.0, 1); // black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderSkybox(*sky, *sky_program, *_camera);

        if(tileMap)
        {
            renderTileMap(*tileMap, *texture_program, *phong_program, *_camera, lights
                          , _grass, _water, _mountain, _square);
        }

        for(auto& model : models)
        {
            render(*texture_program, *phong_program, *_camera, lights, *model);
        }

        if (grid) {
            drawTileType(*grid_program,*_camera, Tile::Type::AOE);
            drawTileType(*grid_program,*_camera, Tile::Type::BUILDING);
            drawGrid(*grid_program, *_camera);
        }

        drawLaser(*grid_program, *_camera);
        drawSelected(*grid_program, *_camera);

        if(_selection_selecting)
        {
            drawSelection(_selection_start, _selection_end, *_camera, *grid_program);
        }

        HUDElementVisitor visitor(*overlay_program, *holstein);
        for(auto t : texts)
        {
            t->accept(visitor);
        }

        showFPS();
        glfwSwapBuffers();
    }
}

glm::vec3 RenderEngine::GetTilePosition(){
    double width = tileMap->getMapWidth();
    glm::vec3 mousepos = get3DPositionFromMousePosition();
    float x = mousepos.x;
    float z = mousepos.z;
    int xx = x;
    int zz = z;

    if(x < 0) {
        if(xx != ((int) (x+0.5f)))
            xx = x -1.0f;
        else
            xx = x -0.5f;
    } else {
        if(xx != ((int) (x+0.5f)))
            xx = x +0.0f;
        else
            xx = x +0.5f;
    }
    if(z < 0) {
        if(zz != ((int) (z+0.5f)))
            zz = z -1.0f;
        else
            zz = z -0.5f;
    } else {
        if(zz != ((int) (z+0.5f)))
            zz = z +0.0f;
        else
            zz = z +0.5f;
    }
    x = xx + 0.5f; //omdat hij in het midden moet
    z = zz + 0.5f; //omdat hij hier ook in het midden moet

    return glm::vec3(x,0,z);
}

glm::vec3 RenderEngine::GetTilePositionFromCoordinates(int xPos, int zPos) {
//misschien nodig misschien niet
}

glm::vec3 RenderEngine::get3DPositionFromMousePosition() {
	int xMouse(0), yMouse(0);
	glfwGetMousePos(&xMouse,&yMouse);

	float w = float(800);
	float h = float(600);
    float x = xMouse;
    float y = h - yMouse;

    GLfloat depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, w, h);
	glm::mat4 tmpView = _camera->viewMatrix();
	glm::mat4 tmpProj = _camera->projectionMatrix();
	glm::vec3 screenPos = glm::vec3(x, y, depth);
	glm::vec3 worldPos = glm::unProject(screenPos, tmpView, tmpProj, viewport);
	return worldPos;
}

glm::vec3 RenderEngine::get3DPositionFromCoordinates(int xPos, int yPos) {
	float w = float(800);
	float h = float(600);
    float x = xPos;
    float y = h - yPos;

    GLfloat depth;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, w, h);
	glm::mat4 tmpView = _camera->viewMatrix();
	glm::mat4 tmpProj = _camera->projectionMatrix();
	glm::vec3 screenPos = glm::vec3(x, y, depth);
	glm::vec3 worldPos = glm::unProject(screenPos, tmpView, tmpProj, viewport);

	return worldPos;
}

void RenderEngine::drawTileType(Program& p, Camera& c, Tile::Type type) {
    int amount = tileMap->getMapAmount();
    int width  = 1;
    int height = 1;
    int length = sqrt(amount);

    int bufferSize = 0;
    for (int row = 0; row < length ; row++) {
        for (int column = 0; column < length; column++) {
            if(tileMap->getType(row,column) == type) {
                bufferSize++;
            }
        }
    }

    bufferSize = bufferSize * 2 * 9;
    GLfloat g_vertex_buffer_data[bufferSize];

    int counter = 0;
    for (int row = 0; row < length ; row++) {
        for (int column = 0; column < length; column++) {
            if(tileMap->getType(row,column) == type) {
                g_vertex_buffer_data[counter++] = -9.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -9.0f + column;

                g_vertex_buffer_data[counter++] = -9.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -10.0f + column;

                g_vertex_buffer_data[counter++] = -10.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -10.0f + column;

                g_vertex_buffer_data[counter++] = -10.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -10.0f + column;

                g_vertex_buffer_data[counter++] = -10.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -9.0f + column;

                g_vertex_buffer_data[counter++] = -9.0f + row;
                g_vertex_buffer_data[counter++] = 0.005f;
                g_vertex_buffer_data[counter++] = -9.0f + column;

            }
        }
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    ProgramContext pc {p};

    glm::mat4 projection = c.projectionMatrix();
    glm::mat4 view = c.viewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = projection * view * model;

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    p.setUniform("MVP", MVP);
    if (Tile::Type::AOE == type)
        p.setUniform("inputColor", glm::vec3(1, 1, 0));
    else if (Tile::Type::BUILDING == type)
        p.setUniform("inputColor", glm::vec3(1, 0, 1));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glDrawArrays(GL_TRIANGLES, 0, counter / 3);

    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
}

void RenderEngine::drawGrid(Program& p, Camera& c)
{
    int amount = tileMap->getMapAmount();
    int width = 1;
    int height = 1;

    int length = sqrt(amount);

    int bufferSize = amount * 4 * 6;
    GLfloat g_vertex_buffer_data[bufferSize];

    int counter = 0;
    for (int row = 0; row < length ; row++) {
        for (int column = 0; column < length; column++) {
            // top

            g_vertex_buffer_data[counter++] = -10.0f + (width * column);
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (width * row);
            g_vertex_buffer_data[counter++] = -10.0f + (width * column) + width;
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (width * row);

            // right
            g_vertex_buffer_data[counter++] = -10.0f + (height * column) + height;
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (height * row);
            g_vertex_buffer_data[counter++] = -10.0f + (height * column) + height;
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (height * row) + height;

            // bottom
            g_vertex_buffer_data[counter++] = -10.0f + (width * column) + width;
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (width * row) + width;
            g_vertex_buffer_data[counter++] = -10.0f + (width * column);
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (width * row) + width;

            // left
            g_vertex_buffer_data[counter++] = -10.0f + (height * column);
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (height * row) + height;
            g_vertex_buffer_data[counter++] = -10.0f + (height * column);
            g_vertex_buffer_data[counter++] = 0.03f;
            g_vertex_buffer_data[counter++] = -10.0f + (height * row);
        }
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    ProgramContext pc {p};

    glm::mat4 projection = c.projectionMatrix();
    glm::mat4 view = c.viewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = projection * view * model;

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

    p.setUniform("MVP", MVP);
    p.setUniform("inputColor", glm::vec3(1,0,0));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glDrawArrays(GL_LINES, 0, bufferSize / 3);

    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
}

void RenderEngine::drawLaser(Program& p, Camera& c)
{
    if(laserCount != 0) {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        ProgramContext pc {p};

        glm::mat4 projection = c.projectionMatrix();
        glm::mat4 view = c.viewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 MVP = projection * view * model;

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

        p.setUniform("MVP", MVP);
        p.setUniform("inputColor", glm::vec3(1,0,0));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_LINES, 0, bufferSize / 3);

        glDisableVertexAttribArray(0);

        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
    }
}

void RenderEngine::setLaserData(std::vector<Laser*> lasers) {
    std::vector<Laser*>::iterator iter;

    laserCount = 0;
    int counter = 0;

    for(int i = 0; i < bufferSize; i++)
        g_vertex_buffer_data[i] = 0;

    for (iter = lasers.begin(); iter != lasers.end(); ++iter) {
        if(counter + 1 > bufferSize)
            break;

        laserCount++;

        g_vertex_buffer_data[counter++] = (*iter)->beginPosition.x;
        g_vertex_buffer_data[counter++] = (*iter)->beginPosition.y;
        g_vertex_buffer_data[counter++] = (*iter)->beginPosition.z;

        g_vertex_buffer_data[counter++] = (*iter)->endPosition.x;
        g_vertex_buffer_data[counter++] = (*iter)->endPosition.y;
        g_vertex_buffer_data[counter++] = (*iter)->endPosition.z;
    }
}

void RenderEngine::drawSelected(Program& p, Camera& c)
{
    if(selectedCount != 0) {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        ProgramContext pc {p};

        glm::mat4 projection = c.projectionMatrix();
        glm::mat4 view = c.viewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 MVP = projection * view * model;

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSizeSelected * sizeof(GLfloat), g_vertex_buffer_dataSelected, GL_STATIC_DRAW);

        p.setUniform("MVP", MVP);
        p.setUniform("inputColor", glm::vec3(1,1,1));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_LINES, 0, bufferSizeSelected / 3);

        glDisableVertexAttribArray(0);

        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
    }
}

void RenderEngine::setSelectedData(std::list<std::shared_ptr<Entity>> entities) {
    std::list<std::shared_ptr<Entity>>::iterator iter;
    float y = 0.01f;
    selectedCount = 0;
    int counter = 0;

    for(int i = 0; i < bufferSizeSelected; i++)
        g_vertex_buffer_dataSelected[i] = 0;

    for (iter = entities.begin(); iter != entities.end(); ++iter) {
        if(counter + 1 > bufferSizeSelected)
            break;

        auto spatial = (*iter)->component<SpatialComponent>();
        auto sizeC   = (*iter)->component<SizeComponent>();
        if(spatial != nullptr) { //deze check kan stiekem wel weg omdat Tim hier al het goed is op checkt.
            float x = spatial->position.x;
            float z = spatial->position.z;
            float offSetZ = 0.25;
            float offSetX = 0.25;
            if( sizeC != nullptr ) {
                offSetZ = sizeC->x*0.25;
                offSetX = sizeC->z*0.25;
            }
            selectedCount++;

            g_vertex_buffer_dataSelected[counter++] = x-offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z-offSetZ;

            g_vertex_buffer_dataSelected[counter++] = x+offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z-offSetZ;



            g_vertex_buffer_dataSelected[counter++] = x+offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z-offSetZ;

            g_vertex_buffer_dataSelected[counter++] = x+offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z+offSetZ;



            g_vertex_buffer_dataSelected[counter++] = x+offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z+offSetZ;

            g_vertex_buffer_dataSelected[counter++] = x-offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z+offSetZ;



            g_vertex_buffer_dataSelected[counter++] = x-offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z+offSetZ;

            g_vertex_buffer_dataSelected[counter++] = x-offSetX;
            g_vertex_buffer_dataSelected[counter++] = y;
            g_vertex_buffer_dataSelected[counter++] = z-offSetZ;

        }
    }
}

void RenderEngine::setGrid(bool useGrid)
{
    grid = useGrid;
}

void RenderEngine::setTileMap(TileMap* tilemap)
{
    tileMap = tilemap;
}

void RenderEngine::setSelection(bool selecting, glm::vec3 start, glm::vec3 end)
{
    _selection_selecting = selecting;
    _selection_start = start;
    _selection_end = end;
}
