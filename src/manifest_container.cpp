#include "manifest_container.h"

void manifest_container::add(std::shared_ptr<Ymir::Manifest> manifest)
{
    _manifests.push_back(manifest);
}

std::unique_ptr<Ymir::StreamReader> manifest_container::read(const std::string& identifier) const
{
    for(auto& manifest : _manifests)
    {
        std::unique_ptr<Ymir::StreamReader> reader = manifest->read(identifier);
        if(reader)
            return reader;
    }
    return nullptr;
}