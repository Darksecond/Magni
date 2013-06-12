#pragma once

#include "Program.h"
#include "resource_loader.h"

class program_resource_loader : public resource_loader
{
public:
    virtual inline std::shared_ptr<void> load(const manifest_container& manifests, const std::string& identifier)
    {
        auto vertStream = manifests.read(identifier + std::string{".vert"});
        auto fragStream = manifests.read(identifier + std::string{".frag"});
        if(vertStream && fragStream)
        {
            Ymir::Shader vert{vertStream->string(), GL_VERTEX_SHADER};
            Ymir::Shader frag{fragStream->string(), GL_FRAGMENT_SHADER};
            std::vector<Ymir::Shader> shaders{};
            shaders.push_back(std::move(vert));
            shaders.push_back(std::move(frag));
            return std::make_shared<Ymir::Program>(shaders);
        }
        return nullptr;
    }
};
