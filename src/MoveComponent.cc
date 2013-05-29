#include "MoveComponent.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

using namespace Ymir;

MoveComponent::MoveComponent(int s, int rSpeed, std::vector<Tile> *tileMap) :
    speed(s),rotationSpeed(rSpeed),tiles(tileMap){

}
