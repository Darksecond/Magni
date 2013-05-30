#include "Laser.h"

using namespace Ymir;

Laser::Laser(glm::vec3 thisBeginPosition, glm::vec3 thisEndPosition, float time) {
    beginPosition = thisBeginPosition;
    endPosition = thisEndPosition;
    duration = time;
}
