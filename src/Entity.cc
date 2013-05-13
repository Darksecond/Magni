#include "Entity.h"

using namespace Ymir;

Entity::Entity(EngineManager& e, const std::string& n, int thisid) : components(), behaviors(), engines(e), parent(nullptr), name(n), id(thisid)
{
}
