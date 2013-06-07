#include "model.h"

model::model(const std::string& name, std::shared_ptr<Ymir::Mesh> mesh, const glm::vec3& position) : game_object(name, position), _mesh(mesh)
{
}

void model::accept(game_object_visitor& v)
{
    v.start_visit(*this);
    for(auto child : _children)
        child->accept(v);
    v.end_visit(*this);
}

std::shared_ptr<Ymir::Mesh> model::mesh()
{
    return _mesh;
}
