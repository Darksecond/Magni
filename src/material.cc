#include "material.h"

material::material(std::shared_ptr<Ymir::Texture> diffuse) : _diffuse(diffuse)
{
}

void material::bind()
{
    _diffuse->bind();
}