#pragma once

#include "Manifest.h"

#include <memory>
#include <vector>
#include <string>
#include <map>

template<typename T>
class DefaultResourceLoader
{
public:
    T load(StreamReader& stream)
    {
        return T::fromStream(stream);
    }
};

template<typename T>
class DefaultResourceCache
{
    std::map<std::string, std::weak_ptr<T>> cache;
public:
    void insert(const std::string& identifier, std::shared_ptr<T> resource)
    {
        cache.insert(std::make_pair(identifier, resource));
    }
    
    std::shared_ptr<T> get(const std::string& identifier)
    {
        std::shared_ptr<T> resource{nullptr};
        auto it = cache.find(identifier);
        
        if(it != cache.end() && !it->second.expired())
        {
            resource = it->second.lock();
        }
        return resource;
    }
};

template<typename T>
class DisabledResourceCache
{
public:
    void insert(const std::string& identifier, std::shared_ptr<T> resource) {}
    std::shared_ptr<T> get(const std::string& identifier) { return nullptr; }
};

//TODO add CacheStrategy or something as extra option to the template
template<typename T, typename Loader = DefaultResourceLoader<T>, typename Cache = DefaultResourceCache<T>>
class ResourceManager
{
    std::vector<std::unique_ptr<Manifest>> manifests;
    Cache cache;
    
    std::unique_ptr<StreamReader> read(const std::string& identifier);
public:
    
    std::shared_ptr<T> resource(const std::string& identifier);
    
    void addManifest(std::unique_ptr<Manifest>&& manifest);
};

//TEMPLATE METHODS
template<typename T, typename Loader, typename Cache>
std::shared_ptr<T> ResourceManager<T,Loader,Cache>::resource(const std::string& identifier)
{
    std::shared_ptr<T> resource = cache.get(identifier);
    if(!resource)
    {
        //allocate resource
        //TODO instead of an unique_ptr, throw an 'file not found' exception?
        std::unique_ptr<StreamReader> stream = read(identifier);
        if(stream)
        {
            Loader loader;
            resource = std::make_shared<T>( std::move(loader.load(*stream)) );
            cache.insert(identifier, resource);
        }
    }
    return resource;
}

template<typename T, typename Loader, typename Cache>
void ResourceManager<T,Loader,Cache>::addManifest(std::unique_ptr<Manifest>&& manifest)
{
    manifests.push_back(std::move(manifest));
}

template<typename T, typename Loader, typename Cache>
std::unique_ptr<StreamReader> ResourceManager<T, Loader, Cache>::read(const std::string& identifier)
{
    for(auto& manifest : manifests)
    {
        std::unique_ptr<StreamReader> reader = manifest->read(identifier);
        if(reader)
            return reader;
    }
    return std::unique_ptr<StreamReader>{nullptr};
}