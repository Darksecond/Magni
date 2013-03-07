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
        std::shared_ptr<Cubemap> load(const ManifestContainer& manifests, const std::string& identifier)
        {
            std::vector<Bitmap> bitmaps;
            
            for(int i = 1; i <= 6; ++i)
            {
                std::stringstream ident;
                ident << i;
                ident << "_";
                ident << identifier;
                
                auto stream = manifests.read(ident.str());
                
                stbi_io_callbacks callbacks;
                callbacks.eof = [](void* user) -> int
                {
                    StreamReader* reader = reinterpret_cast<StreamReader*>(user);
                    return reader->eof() ? 0 : 1;
                };
                callbacks.read = [](void* user, char* data, int size) -> int
                {
                    StreamReader* reader = reinterpret_cast<StreamReader*>(user);
                    return static_cast<int>(reader->read(data, size));
                };
                callbacks.skip = [](void* user, unsigned n) -> void
                {
                    StreamReader* reader = reinterpret_cast<StreamReader*>(user);
                    reader->skip(n);
                };
                
                int width, height, channels;
                uint8_t* pixels = stbi_load_from_callbacks(&callbacks, reinterpret_cast<void*>(stream.get()), &width, &height, &channels, 0);
                if(!pixels)
                    throw std::runtime_error(stbi_failure_reason());
                
                bitmaps.emplace_back(width, height, (Bitmap::Format)channels, pixels);
                
                stbi_image_free(pixels);
            }
            
            return std::make_shared<Cubemap>(bitmaps);
        }
    };
};