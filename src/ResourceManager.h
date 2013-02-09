#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

//TODO move into it's own file
class StreamReader
{
public:
    
    /**
     * \param s buffer to read into
     * \param max_size size of the buffer, maximum amount to read
     * \return amount of bytes read
     */
    virtual size_t read(char* s, const size_t max_size) = 0;
    
    /**
     * \param amount amount of bytes to skip
     */
    virtual void skip(const size_t amount) = 0;
    
    /**
     * \return true if we are at the end of the stream
     */
    virtual bool eof() const = 0;
    
};

//TODO move into it's own file
class Manifest
{
public:
    virtual std::unique_ptr<StreamReader> read(const std::string& identifier) = 0;
};

template<typename T>
class DefaultResourceLoader
{
public:
    T load(StreamReader& stream)
    {
        return T::fromStream(stream);
    }
};

template<typename T, typename Loader = DefaultResourceLoader<T>>
class ResourceManager
{
    std::vector<std::unique_ptr<Manifest>> manifests;
    std::map<std::string, std::weak_ptr<T>> cache;
    
    std::unique_ptr<StreamReader> read(const std::string& identifier);
public:
    
    std::shared_ptr<T> resource(const std::string& identifier);
    
    void addManifest(std::unique_ptr<Manifest>&& manifest);
};

//TEMPLATE METHODS
template<typename T, typename Loader>
std::shared_ptr<T> ResourceManager<T,Loader>::resource(const std::string& identifier)
{
    std::shared_ptr<T> resource{nullptr};
    auto it = cache.find(identifier);
    if(it != cache.end() && !it->second.expired())
    {
        resource = it->second.lock();
    }
    else
    {
        //allocate resource
        //TODO instead of an unique_ptr, throw an 'file not found' exception?
        std::unique_ptr<StreamReader> stream = read(identifier);
        if(stream)
        {
            Loader loader;
            resource = std::make_shared<T>( std::move(loader.load(*stream)) );
            cache.insert( make_pair(identifier, resource) );
        }
    }
    return resource;
}

template<typename T, typename Loader>
void ResourceManager<T,Loader>::addManifest(std::unique_ptr<Manifest>&& manifest)
{
    manifests.push_back(std::move(manifest));
}

template<typename T, typename Loader>
std::unique_ptr<StreamReader> ResourceManager<T, Loader>::read(const std::string& identifier)
{
    for(auto& manifest : manifests)
    {
        std::unique_ptr<StreamReader> reader = manifest->read(identifier);
        if(reader)
            return reader;
    }
    return std::unique_ptr<StreamReader>{nullptr};
}