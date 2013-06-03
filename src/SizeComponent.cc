#include "SizeComponent.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

using namespace Ymir;

SizeComponent::SizeComponent(float offset, int x, int z)
{
    offSetSelected = offset;
    x = x;
    z = z;
}
