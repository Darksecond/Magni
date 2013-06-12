#include "resource_cache.h"

#include <string>

void resource_cache::insert(const std::string& identifier, const std::string& type, std::shared_ptr<void> resource)
{
    _cache.insert(std::make_pair(std::make_pair(identifier, type), resource));
}

std::shared_ptr<void> resource_cache::get(const std::string& identifier, const std::string& type)
{
    std::shared_ptr<void> resource = nullptr;
    auto it = _cache.find(std::make_pair(identifier, type));
    
    if(it != _cache.end() && !it->second.expired())
    {
        resource = it->second.lock();
    }
    return resource;
}
