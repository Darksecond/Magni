#pragma once

#include "resource_loader.h"
#include "manifest_container.h"
#include "Cubemap.h"
#include "Bitmap.h"
#include "StreamReader.h"
#include "stb_image.h"

#include <memory>
#include <vector>
#include <sstream>

class cubemap_resource_loader : public resource_loader
{
public:
    virtual std::shared_ptr<void> load(const manifest_container& manifests, const std::string& identifier);
};