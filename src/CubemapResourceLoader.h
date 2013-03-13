#pragma once

#include "ResourceManager.h"
#include "Cubemap.h"
#include "Bitmap.h"

#include "StreamReader.h"
#include "stb_image.h"

#include <memory>
#include <vector>
#include <sstream>

namespace Ymir
{
    class CubemapResourceLoader
    {
    public:
        std::shared_ptr<Cubemap> load(const ManifestContainer& manifests, const std::string& identifier);
    };
};
