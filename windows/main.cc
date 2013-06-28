#include "../../src/game.h"
#include <iostream>

static std::string ResourceDirectory()
{
    return "./";
}

int main(int argc, char* argv[])
{
    try{
        game& g = game::instance();
        g.boot(ResourceDirectory());
        g.build();
        g.run();
        g.shutdown();
    } catch(std::runtime_error e) {
        std::cout << e.what();
    }

    return 0;
}
