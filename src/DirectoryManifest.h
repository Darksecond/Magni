#pragma once

#include "FileStreamReader.h"
#include "Manifest.h"

#include <memory>
#include <string>
#include <fstream>

namespace Ymir
{
    class DirectoryManifest : public Manifest
    {
        const std::string directory;
    public:
        DirectoryManifest(const std::string& directory);
        
        std::unique_ptr<StreamReader> read(const std::string& identifier);
    };
};