#include "UniqueNumberGenerator.h"

using namespace Ymir;

UniqueNumberGenerator::UniqueNumberGenerator() {
    uniqueNumber = 0;
}

int UniqueNumberGenerator::getNewUniqueNumber() {
    return uniqueNumber++;
}

int UniqueNumberGenerator::flip(int id)
{
    if(id < half_way)
    {
        return id+half_way;
    }
    else
    {
        return id-half_way;
    }
}