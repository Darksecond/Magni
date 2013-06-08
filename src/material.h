#pragma once

#include "Texture.h"

#include <memory>

class material
{
public:
    material(std::shared_ptr<Ymir::Texture> diffuse);
    
    //TODO render OR bind OR draw OR diffuse() or begin
    
    void bind();
private:
    std::shared_ptr<Ymir::Texture> _diffuse;
};