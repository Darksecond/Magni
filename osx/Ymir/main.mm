#import <Foundation/Foundation.h>

#include "Application.h"

using namespace Ymir;

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

int main(int argc, char* argv[])
{
    Application application = Application();
    application.createManagers(ResourceDirectory() + "/resources");
    application.createEngines();
    application.buildGame();
    application.waitNetwork();
    application.runGame();

    return 0;
}