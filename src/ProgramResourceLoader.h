#pragma once

#include "Program.h"
#include "ResourceManager.h"

namespace Ymir
{
    class ProgramResourceLoader
    {
    public:
        std::shared_ptr<Program> load(const ManifestContainer& manifests, const std::string& identifier)
        {
            auto vertStream = manifests.read(identifier + std::string{".vert"});
            auto fragStream = manifests.read(identifier + std::string{".frag"});
            if(vertStream && fragStream)
            {
                Shader vert{vertStream->string(), GL_VERTEX_SHADER};
                Shader frag{fragStream->string(), GL_FRAGMENT_SHADER};
                std::vector<Shader> shaders{};
                shaders.push_back(std::move(vert));
                shaders.push_back(std::move(frag));
                return std::make_shared<Program>(shaders);
            }
            return nullptr;
        }
    };
};