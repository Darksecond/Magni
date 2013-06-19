#pragma once

#include "Texture.h"
#include "Program.h"

#include <memory>

class material
{
public:
    material(std::shared_ptr<Ymir::Texture> diffuse);
    material(const material&) = delete;
    material(material&&) = delete;
    material& operator=(const material&) = delete;
    material& operator=(material&&) = delete;
    
    //TODO render OR bind OR draw OR diffuse() or begin
    
    void bind(Ymir::Program&);
private:
    std::shared_ptr<Ymir::Texture> _diffuse;
};