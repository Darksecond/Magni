#pragma once

#include "ResourceManager.h"

#include <memory>
#include <string>
#include <fstream>

namespace Ymir
{
    //TODO move into it's own file
    class FileStreamReader : public StreamReader
    {
        std::ifstream stream;
    public:
        FileStreamReader(const std::string& filename, const std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary);
        size_t read(char* s, const size_t max_size);
        void skip(const size_t amount);
        bool eof() const;
        size_t size();
    };

    class DirectoryManifest : public Manifest
    {
        const std::string directory;
    public:
        DirectoryManifest(const std::string& directory);
        
        std::unique_ptr<StreamReader> read(const std::string& identifier);
    };
};