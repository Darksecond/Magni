#include "behaviour_factory.h"
#include "fpscam_behaviour.h"
#include "blue_door_behaviour.h"
#include "red_door_behaviour.h"
#include "wall_behaviour.h"
#include "blue_key_behaviour.h"
#include "red_key_behaviour.h"

std::shared_ptr<behaviour> behaviour_factory::makeBehaviour(const std::string& identifier)
{
    if(identifier == "fpscam")
        return std::make_shared<fpscam_behaviour>();
    if(identifier == "blue_door")
        return std::make_shared<blue_door_behaviour>();
    if(identifier == "red_door")
        return std::make_shared<red_door_behaviour>();
    if(identifier == "wall")
        return std::make_shared<wall_behaviour>();
    if(identifier == "blue_key")
        return std::make_shared<blue_key_behaviour>();
    if(identifier == "red_key")
        return std::make_shared<red_key_behaviour>();
    return nullptr;
}
