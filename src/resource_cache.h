#pragma once

#include <memory>
#include <map>

class resource_cache
{
public:
    void insert(const std::string& identifier, const std::string& type, std::shared_ptr<void> resource);
    std::shared_ptr<void> get(const std::string& identifier, const std::string& type);
private:
    std::map<std::pair<std::string, std::string>, std::weak_ptr<void>> _cache;
};
