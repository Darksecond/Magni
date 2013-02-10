#include "StreamReader.h"

using namespace Ymir;

std::string StreamReader::string()
{
    std::string contents;
    contents.resize(size());
    read(&contents[0], contents.size());
    return contents;
}