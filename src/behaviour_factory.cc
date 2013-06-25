#include "behaviour_factory.h"
#include "fpscam_behaviour.h"

std::shared_ptr<behaviour> behaviour_factory::makeBehaviour(const std::string& identifier)
{
    if(identifier == "fpscam")
        return std::make_shared<fpscam_behaviour>();
    return nullptr;
}
