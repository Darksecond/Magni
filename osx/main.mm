#import <Foundation/Foundation.h>

#include "game_object.h" //TODO game class
#include "light.h" //TODO game class
#include "game_object_visitor.h" //TODO game class

#include <string>

#include <iostream> //TODO debug

class visitor : public game_object_visitor
{
public:
    virtual void visit(game_object& g){ std::cout << "game_object: " << g.name() << std::endl; }
    virtual void visit(light& l){ std::cout << "light: " << l.name() << std::endl; }
};

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

int main(int argc, char* argv[])
{
    std::shared_ptr<game_object> world = std::make_shared<game_object>("world");
    std::shared_ptr<game_object> player = std::make_shared<game_object>("player");
    std::shared_ptr<light> l = std::make_shared<light>("light");
    world->add(player);
    world->add(l);
    
    visitor v;
    world->accept(v);
    return 0;
}