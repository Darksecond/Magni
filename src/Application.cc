#include "Application.h"

//TODO TEMP
#include "HealthComponent.h"
#include "AttackComponent.h"

using namespace Ymir;

Application::Application() : SCREEN_SIZE(800, 600)
{

}

void Application::createManagers(std::string resourceDirectory)
{
    manifest = std::make_shared<DirectoryManifest>(resourceDirectory);

    textureManager.addManifest(manifest);
    programManager.addManifest(manifest);
    meshManager.addManifest(manifest);
    audioBufferManager.addManifest(manifest);
    cubemapManager.addManifest(manifest);
}

void Application::createEngines()
{
    engines = new EngineManager();
    engines->assign<BehaviorEngine>();
    engines->assign<CollisionEngine>();
    engines->assign<AudioEngine>();
    renderEngine = &engines->assign<RenderEngine>(programManager, textureManager, cubemapManager);


    engines->assign<EnergyEngine>(*renderEngine);

    currencyEngine = &engines->assign<CurrencyEngine>(*renderEngine);

    attackEngine = &engines->assign<AttackEngine>(meshManager, textureManager);
}

void Application::buildGame()
{
    // TODO cleanup ----------------------------------
    std::shared_ptr<Texture> track_tex = textureManager.resource("grass.png");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> unit_mesh = meshManager.resource("unit.obj");
    std::shared_ptr<Mesh> track_mesh = meshManager.resource("track.obj");
    // end cleanup -----------------------------------

    gameplay = new Gameplay(*engines,*currencyEngine, textureManager, meshManager, *renderEngine, SCREEN_SIZE);
    gameplay->createCamera();

    TileMap* tiles __unused = new TileMap(100, 2, 2);

    lastTime = glfwGetTime();
}

void Application::runGame()
{
    // TODO cleanup ----------------------------------
    bool isDone, isDone1, isDone2, isDone3, isDone4, isDone5;
    isDone = true;
    isDone1 = true;
    isDone2 = true;
    isDone3 = true;
    isDone4 = true;
    isDone5 = true;

    std::shared_ptr<Texture> track_tex = textureManager.resource("grass.png");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> unit_mesh = meshManager.resource("untitled.obj");
    std::shared_ptr<Mesh> track_mesh = meshManager.resource("track.obj");

    Scene& scene = gameplay->getScene();
    attackEngine->setScene(&scene);

    //light one (spot)
    Entity& light_one = scene.assign("light one");
    light_one.assign<LightComponent>(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{0.0, 0.25, 0.05});
    light_one.assign<SpatialComponent>(glm::vec3{7.0, 0.0, 0.0});

    //light two (directional)
    Entity& light_two = scene.assign("light two");
    light_two.assign<SpatialComponent>(glm::vec3{0.0, 1.0, 0.0});
    auto light_two_lightc = light_two.assign<LightComponent>(glm::vec3{0.8, 0.8, 0.8});
    light_two_lightc.lightType = LightComponent::LightType::DIRECTIONAL;

//    track
    Entity& track = scene.assign("track");
    track.assign<SpatialComponent>(glm::vec3{0, 0.045, 0});
    track.assign<ModelComponent>(track_mesh, track_tex);

    Entity& unit = scene.assign("unit");
    unit.assign<SpatialComponent>(glm::vec3{0, 0.045, 0});
    unit.assign<ModelComponent>(unit_mesh, t);
    // end cleanup -----------------------------------

    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        double delta = thisTime - lastTime;
        lastTime = thisTime;

        engines->update(-1, delta);
        engines->update(0, delta);
        engines->update(1, delta);

        glfwDisable(GLFW_KEY_REPEAT);

        // TODO cleanup ----------------------------------
        if(glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
            gameplay->updateSelectedEntity(renderEngine->get3DPositionFromMousePosition());
        }

        if(glfwGetKey( 'M' ) == GLFW_PRESS ) {
            gameplay->moveEntity();
        }

        if(glfwGetKey( 'I' ) == GLFW_PRESS && isDone) {
            isDone = false;
            gameplay->buildCentralIntelligenceCore(renderEngine->get3DPositionFromMousePosition());
        }
        if(glfwGetKey( 'O' ) == GLFW_PRESS && isDone1) {
            isDone1 = false;
            gameplay->buildOrbitalDropBeacon(renderEngine->get3DPositionFromMousePosition());
        }
        if(glfwGetKey( 'J' ) == GLFW_PRESS && isDone2) {
            isDone2 = false;
            gameplay->createWorker(renderEngine->get3DPositionFromMousePosition());
        }
        if(glfwGetKey( 'K' ) == GLFW_PRESS && isDone3) {
            isDone3 = false;
            gameplay->createBasicInfantrie(renderEngine->get3DPositionFromMousePosition());
        }
        if(glfwGetKey(GLFW_KEY_DEL) == GLFW_PRESS) {
            Entity* entity = gameplay->getCurrentSelectedEntity();
                gameplay->sellEntity(entity);
        }
        if(glfwGetKey( 'R') == GLFW_PRESS && isDone4) {
            isDone4 = false;
            gameplay->winGame();
        }
        if(glfwGetKey( 'E') == GLFW_PRESS && isDone5) {
            isDone5 = false;
            gameplay->loseGame();
        }
        if(glfwGetKey('Z') == GLFW_PRESS) {
            std::cout << "Current owner is now: " << gameplay->objectOwner << std::endl;
            gameplay->switchOwner(1);
        }
        if(glfwGetKey('X') == GLFW_PRESS) {
            std::cout << "Current owner is now: " << gameplay->objectOwner << std::endl;
            gameplay->switchOwner(2);
        }
        if(glfwGetKey('T') == GLFW_PRESS)
        {
            Entity* attacking_unit = gameplay->getCurrentSelectedEntity();
            Entity* to_be_attacked = gameplay->getEntityAtPosition(renderEngine->get3DPositionFromMousePosition());
            if(attacking_unit && to_be_attacked && attacking_unit != to_be_attacked)
            {
                if(attacking_unit->component<AttackComponent>() && to_be_attacked->component<HealthComponent>())
                {
                    attackEngine->attack(*to_be_attacked, *attacking_unit);
                    std::cout << "Unit: " << attacking_unit->name << " is attacking: " << to_be_attacked->name << std::endl;
                }
            }
        }
        // end cleanup -----------------------------------

        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
    }
}
