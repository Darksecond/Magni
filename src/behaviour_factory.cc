#include "behaviour_factory.h"
#include "fpscam_behaviour.h"
#include "door_behaviour.h"
#include "wall_behaviour.h"

std::shared_ptr<behaviour> behaviour_factory::makeBehaviour(const std::string& identifier)
{
    if(identifier == "fpscam")
        return std::make_shared<fpscam_behaviour>();
    if(identifier == "door")
        return std::make_shared<door_behaviour>();
    if(identifier == "wall")
        return std::make_shared<wall_behaviour>();
    return nullptr;
}
