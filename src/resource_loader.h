#pragma once

#include "manifest_container.h"

#include <memory>

class resource_loader
{
public:
    virtual std::shared_ptr<void> load(const manifest_container& manifests, const std::string& identifier) = 0;
};

template <typename T>
class default_resource_loader : public resource_loader
{
public:
    virtual inline std::shared_ptr<void> load(const manifest_container& manifests, const std::string& identifier)
    {
        auto stream = manifests.read(identifier);
        if(stream)
        {
            return std::make_shared<T>(T::fromStream(*stream));
        }
        return nullptr;
    }
};