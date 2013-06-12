#pragma once

#include "Manifest.h"

#include <vector>
#include <memory>

class manifest_container
{
    std::vector<std::shared_ptr<Ymir::Manifest>> _manifests;
public:
    void add(std::shared_ptr<Ymir::Manifest> manifest);
    std::unique_ptr<Ymir::StreamReader> read(const std::string& identifier) const;
};
