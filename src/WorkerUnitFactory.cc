#include "WorkerUnitFactory.h"

using namespace Ymir;

Entity& WorkerUnitFactory::makeLocal(glm::vec3 position)
{
    gameplay.createWorker();
}

Entity& WorkerUnitFactory::makeRemote(glm::vec3 position, int id)
{
}
