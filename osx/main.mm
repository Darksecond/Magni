#import <Foundation/Foundation.h>

#include "game_object.h" //TODO game class
#include "light.h" //TODO game class
#include "game_object_visitor.h" //TODO game class
#include "spatial_updater_game_object_visitor.h"

#include <string>

#include <iostream> //TODO debug

class visitor : public game_object_visitor
{
public:
    virtual void start_visit(game_object& g){ std::cout << "game_object: " << g.name() << std::endl; }
    virtual void start_visit(light& l){ std::cout << "light: " << l.name() << std::endl; }
};

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

int main(int argc, char* argv[])
{
    std::shared_ptr<game_object> world = std::make_shared<game_object>("world", glm::vec3(0,0,0));
    std::shared_ptr<game_object> player = std::make_shared<game_object>("player", glm::vec3(1,1,1));
    std::shared_ptr<light> l = std::make_shared<light>("light", glm::vec3(2,2,2));
    std::shared_ptr<light> l2 = std::make_shared<light>("light2", glm::vec3(4,4,4));
    world->add(player);
    player->add(l);
    world->add(l2);
    
    visitor v;
    world->accept(v);
    
    spatial_updater_game_object_visitor sv;
    world->accept(sv);
    
    std::cout << l->global().translation().z << std::endl;
    
    return 0;
}