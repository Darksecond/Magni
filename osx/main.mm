#import <Foundation/Foundation.h>

#include "game.h"

#include <string>

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

int main(int argc, char* argv[])
{
    game g;
    g.boot(ResourceDirectory());
    g.build();
    g.run();
    
    return 0;
}