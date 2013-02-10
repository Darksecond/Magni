#pragma once

#include "Manifest.h"

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Ymir
{
    class ManifestContainer
    {
        std::vector<std::shared_ptr<Manifest>> manifests;
    public:
        void add(std::shared_ptr<Manifest> manifest)
        {
            manifests.push_back(manifest);
        }
        
        std::unique_ptr<StreamReader> read(const std::string& identifier) const
        {
            for(auto& manifest : manifests)
            {
                std::unique_ptr<StreamReader> reader = manifest->read(identifier);
                if(reader)
                    return reader;
            }
            return nullptr;
        }
    };
    
    
    template<typename T>
    class DefaultResourceLoader
    {
    public:
        std::shared_ptr<T> load(const ManifestContainer& manifests, const std::string& identifier)
        {
            auto stream = manifests.read(identifier);
            if(stream)
            {
                return std::make_shared<T>(T::fromStream(*stream));
            }
            return nullptr;
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

    
    template<typename T, typename Loader = DefaultResourceLoader<T>, typename Cache = DefaultResourceCache<T>>
    class ResourceManager
    {
        ManifestContainer manifests;
        Cache cache;        
    public:
        std::shared_ptr<T> resource(const std::string& identifier);
        void addManifest(std::shared_ptr<Manifest> manifest);
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
            Loader loader;
            resource = loader.load(manifests, identifier);
            if(resource)
            {
                cache.insert(identifier, resource);
            }
        }
        return resource;
    }

    template<typename T, typename Loader, typename Cache>
    void ResourceManager<T,Loader,Cache>::addManifest(std::shared_ptr<Manifest> manifest)
    {
        manifests.add(manifest);
    }
};