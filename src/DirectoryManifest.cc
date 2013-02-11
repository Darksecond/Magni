#include "DirectoryManifest.h"

using namespace Ymir;

DirectoryManifest::DirectoryManifest(const std::string& dir) : directory{dir}
{
}

std::unique_ptr<StreamReader> DirectoryManifest::read(const std::string& identifier)
{
    std::string seperator = "/";
    std::string file = directory + seperator + identifier;
    
    return std::unique_ptr<FileStreamReader>{new FileStreamReader{file}};
}