#pragma once

#include "behaviour.h"

#include <memory>
#include <string>

class behaviour_factory
{
public:
    std::shared_ptr<behaviour> makeBehaviour(const std::string& identifier);
};