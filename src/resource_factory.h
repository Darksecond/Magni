#pragma once

#include "Manifest.h"
#include "resource_cache.h"
#include "manifest_container.h"
#include "resource_loader.h"

#include <memory>
#include <string>
#include <map>

class resource_factory
{
public:
    static resource_factory& instance();
    
    resource_factory(const resource_factory&) = delete;
    resource_factory(resource_factory&&) = delete;
    
    resource_factory& operator=(const resource_factory&) = delete;
    resource_factory& operator=(resource_factory&&) = delete;
    
    template <typename T>
    std::shared_ptr<T> resource(const std::string& id, const std::string& type);
    
    void add_manifest(std::shared_ptr<Ymir::Manifest> manifest);
    void add_loader(const std::string& type, std::shared_ptr<resource_loader> loader);
    inline manifest_container& manifests() { return _manifests; }
private:
    resource_factory();
    
    resource_cache _cache;
    manifest_container _manifests;
    
    std::map<std::string, std::shared_ptr<resource_loader>> _loaders;
};

//TEMPLATE & INLINED METHODS
template <typename T>
std::shared_ptr<T> resource_factory::resource(const std::string& identifier, const std::string& type)
{
    std::shared_ptr<void> resource = _cache.get(identifier, type);
    if(!resource)
    {
        auto it = _loaders.find(type);
        if(it != _loaders.end())
        {
            std::shared_ptr<resource_loader> loader = it->second;
            resource = loader->load(_manifests, identifier);
            if(resource)
            {
                _cache.insert(identifier, type, resource);
            }
        }
    }
    return std::static_pointer_cast<T>(resource);
}
