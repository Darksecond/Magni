#include "material.h"

material::material(std::shared_ptr<Ymir::Texture> diffuse) : _diffuse(diffuse)
{
}

void material::bind(Ymir::Program& program)
{
    try
    {
        program.setUniform("diffuse", 0);
        _diffuse->bind(0);
    } catch(std::runtime_error e) {}
}