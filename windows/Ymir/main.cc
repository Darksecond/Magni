#include "../../src/Application.h"
#include <iostream>

using namespace Ymir;

static std::string ResourceDirectory()
{
    return "./";
}

int main(int argc, char* argv[])
{
    try{
        Application application = Application();
        application.createManagers(ResourceDirectory());
        application.createEngines();
        application.buildGame();
        application.runGame();
    } catch(std::runtime_error e) {
        std::cout << e.what();
    }

    return 0;
}
