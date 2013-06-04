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
    renderEngine = &engines->assign<RenderEngine>(programManager, textureManager, cubemapManager, meshManager);

    engines->assign<EnergyEngine>(*renderEngine);

    currencyEngine = &engines->assign<CurrencyEngine>(*renderEngine);

    moveEngine     = &engines->assign<MoveEngine>();
    attackEngine = &engines->assign<AttackEngine>(meshManager, textureManager);
    hudEngine = &engines->assign<HUDEngine>(*renderEngine, textureManager);
    gameHudEngine = &engines->assign<GameHUDEngine>(*hudEngine);
}

void Application::buildGame()
{
    gameplay = new Gameplay(*engines,*currencyEngine, textureManager, meshManager, *renderEngine, SCREEN_SIZE, *attackEngine,*moveEngine,*hudEngine);
    gameplay->createCamera();
    gameplay->ghe = gameHudEngine;

    hudEngine->scene = &gameplay->getScene();

    TileMap* tiles = new TileMap(400, 20, 20); //400 want 20 * 20
    gameplay->setTileMap(tiles);

    for(int i = 1; i < 5; i++)
        for(int j = 1; j < 5; j++)
            tiles->setType(i, j, Tile::Type::MOUNTAIN);

    for(int i = 1; i < 5; i++)
        for(int j = 1; j < 5; j++)
            tiles->setType(i + 14, j + 14, Tile::Type::MOUNTAIN);

    tiles->setType(1, 1, Tile::Type::NONE);
    tiles->setType(4, 4, Tile::Type::NONE);

    tiles->setType(15, 15, Tile::Type::NONE);
    tiles->setType(18, 18, Tile::Type::NONE);

    tiles->setType(8, 8, Tile::Type::WATER);
    tiles->setType(9, 8, Tile::Type::WATER);
    tiles->setType(8, 9, Tile::Type::WATER);
    tiles->setType(9, 9, Tile::Type::WATER);
    tiles->setType(10, 9, Tile::Type::WATER);
    tiles->setType(9, 10, Tile::Type::WATER);
    tiles->setType(10, 10, Tile::Type::WATER);
    tiles->setType(10, 11, Tile::Type::WATER);
    tiles->setType(11, 10, Tile::Type::WATER);
    tiles->setType(11, 11, Tile::Type::WATER);
    tiles->setType(11, 12, Tile::Type::WATER);
    tiles->setType(12, 11, Tile::Type::WATER);
    tiles->setType(12, 12, Tile::Type::WATER);

    tiles->setType(11, 9, Tile::Type::WATER);
    tiles->setType(9, 11, Tile::Type::WATER);

}

void Application::waitNetwork()
{
    //send HELLO message
    NetworkPacket hello(0, Gameplay::HELLO);
    gameplay->client->write(hello.char_array(), hello.size());

    std::shared_ptr<Text> wait_text = std::make_shared<Text>("WAITING FOR PLAYER", glm::vec2{200, 300}, 20);
    renderEngine->addText(wait_text);

    bool done = false;
    while(!done)
    {
        net::Address sender;
        unsigned char buffer[512];

        unsigned long bytes_received = gameplay->client->socket.Receive( sender, buffer, 512);

        if(bytes_received)
        {
            NetworkPacket np(buffer);
            if(np.type() == Gameplay::HELLO)
            {
                gameplay->playerNumber = 1;
                gameplay->otherPlayerNumber = 2;
                NetworkPacket player (0, Gameplay::PLAYER);
                gameplay->client->write(player.char_array(), player.size());
                done = true;
            }
            else if(np.type() == Gameplay::PLAYER)
            {
                gameplay->playerNumber = 2;
                gameplay->otherPlayerNumber = 1;
                done = true;
            }
        }
        else if(glfwGetKey('0') == GLFW_PRESS)
        {
            gameplay->playerNumber = 1;
            gameplay->otherPlayerNumber = 2;
            done = true;
        }

        engines->update(1, 0);
    }

    renderEngine->texts.remove(wait_text);

}

void Application::runGame()
{
    std::shared_ptr<Texture> track_tex = textureManager.resource("grass.png");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> track_mesh = meshManager.resource("track.obj");

    Scene& scene = gameplay->getScene();
    attackEngine->setScene(&scene);
    gameplay->updateCameraStart();

    //light one (spot)
    Entity& light_one = scene.assign("light one");
    light_one.assign<LightComponent>(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{1.0, 0.0, 0.00});
    light_one.assign<SpatialComponent>(glm::vec3{0.0, 5.0, 0.0});


//    track
    Entity& track = scene.assign("track");
    //track.assign<SpatialComponent>(glm::vec3{0, 0, 0});
    //track.assign<ModelComponent>(track_mesh, track_tex);

    gameplay->buildCentralIntelligenceCore();

    // end cleanup -----------------------------------

    Timer* checkDefeatTimer = new Timer(2);
    gameplay->drawGrid(true);

    //HUD
    gameHudEngine->addGroup("empty");

    auto cicore_group = gameHudEngine->addGroup("ciCore");
    cicore_group->addItem("wooden-crate.jpg", *gameplay, &Gameplay::createWorker);

    auto worker_group = gameHudEngine->addGroup("worker");
    worker_group->addItem("wooden-crate.jpg", *gameplay, &Gameplay::createOrbitalDropBeacon);

    auto odb_group = gameHudEngine->addGroup("odb");
    odb_group->addItem("wooden-crate.jpg", *gameplay, &Gameplay::createBasicInfantrie);

    gameHudEngine->activateGroup("empty");
    //END HUD

    lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        double delta = thisTime - lastTime;
        lastTime = thisTime;

        gameplay->updateTimer(delta);

        engines->update(-1, delta);
        engines->update(0, delta);
        engines->update(1, delta);

        gameplay->updateSelectedEntity(hudEngine->selectedEntity().get());
        if(!hudEngine->selectedEntity() || gameplay->currentlyBuildingEntity != nullptr)
            gameHudEngine->activateGroup("empty");
        else if(hudEngine->selectedEntity()->name == "ACiCore")
            gameHudEngine->activateGroup("ciCore");
        else if(hudEngine->selectedEntity()->name == "worker")
            gameHudEngine->activateGroup("worker");
        else if(hudEngine->selectedEntity()->name == "OrbitalDropBeacon")
            gameHudEngine->activateGroup("odb");
        else
            gameHudEngine->activateGroup("empty");

        glfwEnable(GLFW_KEY_REPEAT);
        // TODO cleanup ----------------------------------
        if(glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
            //gameplay->updateSelectedEntity(renderEngine->get3DPositionFromMousePosition());
            gameplay->processBuildingUnits(true);
        }
        else
        {
            gameplay->processBuildingUnits(false);
        }
        if(glfwGetKey( 'M' ) == GLFW_PRESS ) {
            gameplay->moveEntity();
        }
        if(glfwGetKey( 'O' ) == GLFW_PRESS) {
            gameplay->buildOrbitalDropBeacon(renderEngine->GetTilePosition());
        }
        if(glfwGetKey( 'J' ) == GLFW_PRESS) {
            gameplay->createWorker(renderEngine->GetTilePosition());
        }
        if(glfwGetKey( 'K' ) == GLFW_PRESS) {
            gameplay->createBasicInfantrie(renderEngine->GetTilePosition());
        }
        if(glfwGetKey( 'L' ) == GLFW_PRESS) {
            gameplay->buildTower(renderEngine->GetTilePosition());
        }
        if(glfwGetKey(GLFW_KEY_DEL) == GLFW_PRESS) {
            gameplay->sellEntity();
        }
        if(glfwGetKey( 'G' ) == GLFW_PRESS) {
            gameplay->drawGrid(true);
        } else {
            gameplay->drawGrid(false);
        }
        if(glfwGetKey( 'R') == GLFW_PRESS) {
            gameplay->winGame();
        }
        if(glfwGetKey( 'E' ) == GLFW_PRESS) {
            gameplay->loseGame();
        }
        if(glfwGetKey('T') == GLFW_PRESS) {
            gameplay->attackEntity();
        }
        if(glfwGetKey('6') == GLFW_PRESS) {
            gameplay->TestFollowPath();
        }
        checkDefeatTimer->update(delta);

        if (checkDefeatTimer->reached()) {
            checkDefeatTimer->reset();

            if (gameplay->centralIntelligenceCoreDestoyed()) {
                gameplay->loseGame();
            }
        }

        gameplay->automaticAttackCheck();
        gameplay->client->readReal();
        gameplay->updateLaserDataToRenderEngine();
        gameplay->updateSelectedDataToRenderEngine();

        // end cleanup -----------------------------------

        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
    }
}
