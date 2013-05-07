#include "UniqueNumberGenerator.h"

using namespace Ymir;

UniqueNumberGenerator::UniqueNumberGenerator() {
    uniqueNumber = 0;
}

int UniqueNumberGenerator::getNewUniqueNumber() {
    return uniqueNumber++;
}
