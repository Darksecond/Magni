#include "../../src/Application.h"

using namespace Ymir;

static std::string ResourceDirectory()
{
    return "./";
}

int main(int argc, char* argv[])
{
    Application application = Application();
    application.createManagers(ResourceDirectory());
    application.createEngines();
    application.buildGame();
    application.runGame();

    return 0;
}
